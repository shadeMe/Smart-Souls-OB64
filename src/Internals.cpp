#include "Internals.h"
#include "Config.h"

Candidate::Candidate(TESSoulGem* SoulGem) :
	SoulGem(SoulGem),
	BaseContainerItem(nullptr),
	InventoryChange(nullptr)
{}

inline bool Candidate::HasInventory() const
{
	return InventoryChange != nullptr;
}

std::uint16_t Candidate::GetCount() const
{
	// The calculation of item counts is inconsistent and buggy in the game code
	// due to the interactions b'ween the base container counts, inventory changes
	// delta counts and counts in the extra data lists.
	// We'll just make the same assumption that the game appears to make (seen in the
	// GetItemCount console command; which by all accounts breaks several corner cases),
	// i.e., the delta count in the inventory changes correctly accounts for any extra
	// data-based counts in its stacks.
	const auto BaseCount{ static_cast<std::int32_t>(BaseContainerItem ? abs(BaseContainerItem->i_count) : 0) };
	const auto InventoryCount{ InventoryChange ? InventoryChange->iNumber : 0 };

	const std::uint32_t Total{ std::clamp(BaseCount + InventoryCount, 0, static_cast<std::int32_t>(std::numeric_limits<std::uint16_t>::max())) };
	return static_cast<std::uint16_t>(Total);
}

void SoulBucket::Enroll(TESSoulGem* SoulGem, ContainerObject* BaseContainerItem)
{
	auto Match{ Store.find(SoulGem) };
	if (Match != Store.end())
	{
		if (Match->second.BaseContainerItem != nullptr)
			REX::WARN("Soulgem '{}' already enrolled with base container entry {:16X}", SoulGem->cFullName.pString, Match->second.BaseContainerItem);

		Match->second.BaseContainerItem = BaseContainerItem;
	}
	else
	{
		Candidate NewCandidate(SoulGem);
		NewCandidate.BaseContainerItem = BaseContainerItem;
		Store.emplace(SoulGem, NewCandidate);
	}
}

void SoulBucket::Enroll(TESSoulGem* SoulGem, ItemChange* InventoryChange)
{
	auto Match{ Store.find(SoulGem) };
	if (Match != Store.end())
	{
		if (Match->second.InventoryChange != nullptr)
			REX::WARN("Soulgem '{}' already enrolled with inventory change entry {:16X}", SoulGem->cFullName.pString, Match->second.InventoryChange);
		Match->second.InventoryChange = InventoryChange;
	}
	else
	{
		Candidate NewCandidate(SoulGem);
		NewCandidate.InventoryChange = InventoryChange;
		Store.emplace(SoulGem, NewCandidate);
	}
}

void SoulBucket::Enroll(TESSoulGem* SoulGem, ContainerObject* BaseContainerItem, ItemChange* InventoryChange)
{
	Enroll(SoulGem, BaseContainerItem);
	Enroll(SoulGem, InventoryChange);
}

SoulBucket SoulBucket::Select(ESelectMode Mode) const
{
	SoulBucket Out;

	for (const auto& Itr : Store)
	{
		switch (Mode)
		{
		case ESelectMode::BaseContainerOnly:
		{
			if (Itr.second.BaseContainerItem != nullptr && Itr.second.InventoryChange == nullptr)
				Out.Enroll(Itr.second.SoulGem, Itr.second.BaseContainerItem);
			break;
		}
		case ESelectMode::InventoryOnly:
		{
			if (Itr.second.BaseContainerItem == nullptr && Itr.second.InventoryChange != nullptr)
				Out.Enroll(Itr.second.SoulGem, Itr.second.InventoryChange);
			break;
		}
		case ESelectMode::BaseContainerAndInventory:
		{
			if (Itr.second.BaseContainerItem != nullptr && Itr.second.InventoryChange != nullptr)
				Out.Enroll(Itr.second.SoulGem, Itr.second.BaseContainerItem, Itr.second.InventoryChange);
			break;
		}
		}
	}

	return Out;
}

SoulBucket::BucketT::const_iterator SoulBucket::begin() const
{
	return Store.cbegin();
}

SoulBucket::BucketT::const_iterator SoulBucket::end() const
{
	return Store.cend();
}

FinalCandidates::FinalCandidates() :
	BaseContainer(nullptr),
	Inventory(nullptr)
{}

namespace
{
	TESSoulGem* BoundObjectAsSoulGem(TESBoundObject* Object)
	{
		// Since the dynamic cast wrapper isn't working, let's just cheat.
		return Object->cFormType == FormType::SLGM_ID ? reinterpret_cast<TESSoulGem*>(Object) : nullptr;
	}

	SOUL_LEVEL GetCreatureSoulLevel(TESBoundObject* Creature)
	{
		using func_t = decltype(&GetCreatureSoulLevel);
		static REL::Relocation<func_t> func{ REL::Offset(0x6868FE0) };
		return func(Creature);
	}

	void EnumerateBaseContainer(TESContainer* Container, SoulBucket* OutBucket)
	{
		for (auto Itr{ &Container->objectList }; Itr != nullptr && Itr->m_item != nullptr; Itr = Itr->m_pkNext)
		{
			auto SoulGem{ BoundObjectAsSoulGem(Itr->m_item->pObj) };
			if (SoulGem != nullptr)
				OutBucket->Enroll(SoulGem, Itr->m_item);
		}
	}

	void EnumerateInventoryChanges(InventoryChanges* Changes, SoulBucket* OutBucket)
	{
		for (auto Itr{ Changes->pListofChanges }; Itr != nullptr && Itr->m_item != nullptr; Itr = Itr->m_pkNext)
		{
			auto SoulGem{ BoundObjectAsSoulGem(Itr->m_item->pContainerObj) };
			if (SoulGem != nullptr)
				OutBucket->Enroll(SoulGem, Itr->m_item);
		}
	}

	struct SoulSacrificeChecks
	{
		bool Sentience;
		bool Capacity;

		// Index of ItemChange::pExtraObjectList that corresponds to the
		// stack that passed the (capacity) check, if any. Can be -1 if there
		// are no stacks (a singleton item).
		std::optional<std::size_t> InventoryStackId;

		SoulSacrificeChecks() :
			Sentience(false),
			Capacity(false),
			InventoryStackId(std::nullopt)
		{}

		inline bool Passed() const {
			return Sentience && Capacity;
		}
	};

	bool CheckSentience(const Candidate& Candidate, const bool IsSentient, SoulSacrificeChecks* OutChecks)
	{
		const auto IsBlackSoulGem{ Candidate.SoulGem == TESSoulGem::BlackSoulGem.get() };
		if (IsSentient)
			OutChecks->Sentience = IsBlackSoulGem;
		else if (Config::RestrictBlackSoulGemsToNpcs.GetValue())
			OutChecks->Sentience = !IsBlackSoulGem;
		else
			OutChecks->Sentience = true;

		return OutChecks->Sentience;
	}

	bool CompareSoulValues(const SOUL_LEVEL CandidateCurrent, const SOUL_LEVEL CandidateCapacity, const SOUL_LEVEL Target)
	{
		const auto CurrentValue{ TESSoulGem::GetSoulLevelValue(CandidateCurrent) };
		const auto MaxValue{ TESSoulGem::GetSoulLevelValue(CandidateCapacity) };
		const auto TargetValue{ TESSoulGem::GetSoulLevelValue(Target) };

		switch (Config::GetSoulTrapMode())
		{
		case Config::ESoulTrapMode::Default:
		{
			// Allow souls that are either the same as or smaller than capacity.
			// Only consider currently empty soulgems.
			if (CandidateCurrent != SOUL_LEVEL::SOUL_NONE)
				return false;
			return MaxValue >= TargetValue;
		}
		case Config::ESoulTrapMode::ExactSoulGem:
		{
			// Allow souls that are exactly the same size as capacity.
			// Only consider currently empty soulgems.
			if (CandidateCurrent != SOUL_LEVEL::SOUL_NONE)
				return false;
			return MaxValue == TargetValue;
		}
		case Config::ESoulTrapMode::UpgradeSoulGem:
		{
			// Allow souls that are larger than the current occupant and
			// at most the capacity.
			return TargetValue > CurrentValue && TargetValue <= MaxValue;
		}
		default:
			return false;
		}
	}

	bool CheckCapacity(const Candidate& Candidate, const SOUL_LEVEL TargetSoulLevel, SoulSacrificeChecks* OutChecks)
	{
		OutChecks->Capacity = false;
		OutChecks->InventoryStackId = std::nullopt;

		auto BaseSoulLevel{ static_cast<SOUL_LEVEL>(Candidate.SoulGem->cCurrentSoul) };
		auto BaseSoulCapacity{ static_cast<SOUL_LEVEL>(Candidate.SoulGem->cSoulCapacity) };

		const auto Prefilled{ BaseSoulLevel != SOUL_LEVEL::SOUL_NONE };
		if (Config::UsePrefilledGems.GetValue() || Prefilled)
			return false;

		// If the total resolved count is not positive, nothing to do here.
		if (Candidate.GetCount() == 0)
			return false;

		if (Candidate.HasInventory())
		{
			auto Change{ Candidate.InventoryChange };
			if (Change->pExtraObjectList == nullptr)
			{
				// A singleton item with no extradata stacks, so the base soul level
				// is the one we need to compare against the target.
				OutChecks->Capacity = CompareSoulValues(BaseSoulLevel, BaseSoulCapacity, TargetSoulLevel);
			}
			else
			{
				// Fetch the first stack that can fit our target.
				std::size_t idx{ 0 };
				for (auto Itr{ Change->pExtraObjectList }; Itr != nullptr && Itr->m_item != nullptr; Itr = Itr->m_pkNext)
				{
					auto Stack{ Itr->m_item };
					const auto HasSoul{ Stack->GetExtraData(EXTRA_DATA_TYPE::EXTRA_SOUL) };
					const auto StackSoulLevel{ HasSoul ? Stack->GetSoul() : BaseSoulLevel };

					OutChecks->Capacity = CompareSoulValues(StackSoulLevel, BaseSoulCapacity, TargetSoulLevel);
					if (OutChecks->Capacity)
						break;
					++idx;
				}

				if (OutChecks->Capacity)
					OutChecks->InventoryStackId.emplace(idx);
			}
		}
		else
		{
			OutChecks->Capacity = CompareSoulValues(BaseSoulLevel, BaseSoulCapacity, TargetSoulLevel);
		}

		return OutChecks->Capacity;
	}

	bool SoulSacrificeSorter(const Candidate& First, const Candidate& Second)
	{
		// Special-case Azura's Star - it gets consumed first whenever possible.
		if (First.SoulGem == TESSoulGem::AzurasStar.get())
			return true;
		else
			return First.SoulGem->cSoulCapacity < Second.SoulGem->cSoulCapacity;
	}

	FinalCandidates SelectBestSoulGem(InventoryChanges* Changes, TESObjectREFR* DeadActor)
	{
		using ShortlistT = std::vector<std::pair<Candidate, SoulSacrificeChecks>>;

		const auto TargetSoulLevel{
			DeadActor->cFormType == FormType::CREA_ID ? GetCreatureSoulLevel(DeadActor->data.pObjectReference) : SOUL_LEVEL::SOUL_GRAND
		};
		const auto SentientSoul{
			DeadActor->cFormType == FormType::NPC__ID
		};

		SoulBucket MainBucket;
		ShortlistT BaseContainerOnly, Inventory;

		auto OwnerBaseContainer{ Changes->pRef->HasContainer() };

		// Skip anything that doesn't have a base container.
		// Should never happen in practice.
		if (OwnerBaseContainer == nullptr)
			return FinalCandidates();

		EnumerateBaseContainer(OwnerBaseContainer, &MainBucket);
		EnumerateInventoryChanges(Changes, &MainBucket);

		// Check and separate candidates by separating into two more shortlists.
		for (const auto& Itr : MainBucket)
		{
			SoulSacrificeChecks NoChecksAndBalances;
			
			CheckSentience(Itr.second, SentientSoul, &NoChecksAndBalances);
			if (!NoChecksAndBalances.Sentience)
				continue;

			CheckCapacity(Itr.second, TargetSoulLevel, &NoChecksAndBalances);

			if (!NoChecksAndBalances.Passed())
				continue;

			if (Itr.second.HasInventory())
				Inventory.emplace_back(Itr.second, NoChecksAndBalances);
			else
				BaseContainerOnly.emplace_back(Itr.second, NoChecksAndBalances);
		}

		FinalCandidates Out;

		// Sort the shortlists such that the smallest (in capacity/value) candidate gets picked.
		// Candidates with inventory changes are given priority.
		if (!Inventory.empty())
		{
			std::sort(Inventory.begin(), Inventory.end(), SoulSacrificeSorter);
			auto &First { Inventory.front()};

			// We need to set the inventory stack's soul level to none, other the 
			// subsequent call to ItemChange::SetSoul will just skip over the stack.
			if (First.second.InventoryStackId != std::nullopt)
			{
				auto xList { *First.first.InventoryChange->pExtraObjectList->At(*First.second.InventoryStackId) };
				auto xSoul { reinterpret_cast<ExtraSoul*>(xList->GetExtraData(EXTRA_DATA_TYPE::EXTRA_SOUL)) };
				xSoul->cSoul = SOUL_LEVEL::SOUL_NONE;
			}

			Out.Inventory = First.first.InventoryChange;
		}
		else if (!BaseContainerOnly.empty())
		{
			std::sort(BaseContainerOnly.begin(), BaseContainerOnly.end(), SoulSacrificeSorter);
			Out.BaseContainer = BaseContainerOnly.front().first.SoulGem;
		}

		return Out;
	}

	/*
		- Walk though base container and enumerate soulgems entries
		- Walk though inventory and enumerate soulgems entries
			- Match those with ones already found in base container
		- We'll have a collection of:
			- Items only found in the base container (no changes in the inventory)
			- Items only found in the inventory (none in base container)
			- Items found in both
		- For each candidate
			- Sentience check
			- Capacity check
				- If empty
					- Exact size
					- At least as large
				- If filled
					- Larger than incoming and filled size smaller than incoming
			- For inventory changes, check each stack separately
		- Sort candidates into above buckets and then sort each bucket by smallest capacity
			- Special case the Azura's star
		- Pick the smallest candidate from above
			- For inventory changes, set the viable stack's soul to None
		*/

}
