#include "Internals.h"
#include "Config.h"

#undef min
#undef max

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
	// data-based counts in its stacks. Note that the delta count should includes the 
	// items that do not have an extradata list.
	// 
	// For example, you have 3 empty soul gems in the inventory. They have a single
	// ItemChange entry with a delta count of 3 (assuming that there were none in the base
	// container) and no extradata lists. After capturing a soul in one of them, a new extradata
	// list is allocated for the it (containing an ExtraSoul instance).
	const auto BaseCount{ static_cast<std::int32_t>(BaseContainerItem ? abs(BaseContainerItem->i_count) : 0) };
	const auto InventoryCount{ InventoryChange ? InventoryChange->iNumber : 0 };

	const std::uint32_t Total{ static_cast<std::uint32_t>(std::clamp(BaseCount + InventoryCount, 0, static_cast<std::int32_t>(std::numeric_limits<std::uint16_t>::max()))) };
	return static_cast<std::uint16_t>(Total);
}

void SoulBucket::Enroll(TESSoulGem* SoulGem, ContainerObject* BaseContainerItem)
{
	auto Match{ Store.find(SoulGem) };
	if (Match != Store.end())
	{
		if (Match->second.BaseContainerItem != nullptr)
			REX::WARN("Soulgem '{}' already enrolled with base container entry {:#016X}", SoulGem->cFullName.pString, reinterpret_cast<std::uintptr_t>(Match->second.BaseContainerItem));

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
			REX::WARN("Soulgem '{}' already enrolled with inventory change entry {:#016X}", SoulGem->cFullName.pString, reinterpret_cast<std::uintptr_t>(Match->second.InventoryChange));
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

SoulBucket::BucketT::const_iterator SoulBucket::begin() const
{
	return Store.cbegin();
}

SoulBucket::BucketT::const_iterator SoulBucket::end() const
{
	return Store.cend();
}

std::size_t SoulBucket::size() const
{
	return Store.size();
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

		inline bool Passed() const
		{
			return Sentience && Capacity;
		}
	};

	bool CheckSentience(const Candidate& Candidate, const bool IsSentient, SoulSacrificeChecks* OutChecks)
	{
		// For some reason, the formID of the default form is set to zero.
		const auto IsBlackSoulGem{ (Candidate.SoulGem->iFormID & 0x00FFFFFF) == 0x192 };
		REX::DEBUG("IsSentient: {} | IsBlackSoulGem: {} | RestrictToNPCs: {}", IsSentient, IsBlackSoulGem, Config::RestrictBlackSoulGemsToNpcs.GetValue());

		if (IsSentient)
			OutChecks->Sentience = IsBlackSoulGem;
		else if (Config::RestrictBlackSoulGemsToNpcs.GetValue())
			OutChecks->Sentience = !IsBlackSoulGem;
		else
			OutChecks->Sentience = true;

		REX::DEBUG("\t{}", OutChecks->Sentience ? "PASS" : "FAIL");

		return OutChecks->Sentience;
	}

	bool CompareSoulValues(const SOUL_LEVEL CandidateCurrent, const SOUL_LEVEL CandidateCapacity, const SOUL_LEVEL Target, bool IsAzurasStar)
	{
		const auto CurrentValue{ TESSoulGem::GetSoulLevelValue(CandidateCurrent) };
		const auto MaxValue{ TESSoulGem::GetSoulLevelValue(CandidateCapacity) };
		const auto TargetValue{ TESSoulGem::GetSoulLevelValue(Target) };

		REX::DEBUG("Mode: {} | Current: {} | Capacity: {} | Target: {} | IsAzurasStar: {}", 
				Config::SoulTrapMode.GetValue(), CurrentValue, MaxValue, TargetValue, IsAzurasStar);

		bool Result{ false };

		switch (Config::GetSoulTrapMode())
		{
		case Config::ESoulTrapMode::Default:
		{
			// Allow souls that are either the same as or smaller than capacity.
			// Only consider currently empty soulgems.
			if (CandidateCurrent == SOUL_LEVEL::SOUL_NONE)
				Result = MaxValue >= TargetValue;

			break;
		}
		case Config::ESoulTrapMode::ExactSoulGem:
		{
			// Allow souls that are exactly the same size as capacity.
			// Only consider currently empty soulgems.
			if (CandidateCurrent == SOUL_LEVEL::SOUL_NONE)
				Result = MaxValue == TargetValue || IsAzurasStar;

			break;
		}
		case Config::ESoulTrapMode::UpgradeSoulGem:
		{
			// Allow souls that are larger than the current occupant and
			// at most the capacity.
			Result = TargetValue > CurrentValue && TargetValue <= MaxValue;

			break;
		}
		}

		REX::DEBUG("\t{}", Result ? "PASS" : "FAIL");

		return Result;
	}

	bool CheckCapacity(const Candidate& Candidate, const SOUL_LEVEL TargetSoulLevel, SoulSacrificeChecks* OutChecks)
	{
		OutChecks->Capacity = false;
		OutChecks->InventoryStackId = std::nullopt;

		auto BaseSoulLevel{ static_cast<SOUL_LEVEL>(Candidate.SoulGem->cCurrentSoul) };
		auto BaseSoulCapacity{ static_cast<SOUL_LEVEL>(Candidate.SoulGem->cSoulCapacity) };
		auto IsAzurasStar { Candidate.SoulGem->IsAzurasStar() };

		const auto Prefilled{ BaseSoulLevel != SOUL_LEVEL::SOUL_NONE };
		REX::DEBUG("Item Count: {} | Prefilled: {} | UsePrefilledGems: {}", Candidate.GetCount(), Prefilled, Config::UsePrefilledGems.GetValue());
		if ((Prefilled && !Config::UsePrefilledGems.GetValue()) || Candidate.GetCount() == 0)
		{
			REX::DEBUG("\tFAIL");
			return false;
		}

		if (Candidate.HasInventory())
		{
			auto Change{ Candidate.InventoryChange };
			if (Change->pExtraObjectList == nullptr)
			{
				// A singleton item with no extradata stacks, so the base soul level
				// is the one we need to compare against the target.
				REX::DEBUG("Singleton inventory item ");
				OutChecks->Capacity = CompareSoulValues(BaseSoulLevel, BaseSoulCapacity, TargetSoulLevel, IsAzurasStar);
			}
			else
			{
				// Fetch the first stack that can fit our target.
				REX::DEBUG("Stacks: {}", Change->pExtraObjectList->Count());
				if (Change->pExtraObjectList->Count())
					REX::DEBUG("{}", std::string(70, '-'));
				
				std::size_t Idx{ 0 };
				std::uint32_t ProcessedItemsInStacks {0};
				for (auto Itr{ Change->pExtraObjectList }; Itr != nullptr && Itr->m_item != nullptr; Itr = Itr->m_pkNext)
				{
					auto Stack{ Itr->m_item };
					const auto xSoul{ Stack->GetExtraData(EXTRA_DATA_TYPE::EXTRA_SOUL) };
					const auto StackSoulLevel{ xSoul ? Stack->GetSoul() : BaseSoulLevel };
					const auto StackCount { Stack->GetCount() };

					// TODO: Stacks with multiple items (StackCount > 1) can potentially get 
					// selected here, resulting in every gem in the stack to have the target
					// soul level. This is exploitable and should be fixed at some point -
					// clone the stack, remove the extra count from the clone, reduce the count
					// of the old stack by one, remove the soul extra data from the clone and
					// prepend it to the stack list (so that ItemChange::SetSoul will pick it
					// correctly).
					REX::DEBUG("[{}] xSoul: {:#016X} | StackSoulLevel: {} | StackCount: {}", Idx, reinterpret_cast<std::uintptr_t>(xSoul), static_cast<std::uint8_t>(StackSoulLevel), StackCount);

					OutChecks->Capacity = CompareSoulValues(StackSoulLevel, BaseSoulCapacity, TargetSoulLevel, IsAzurasStar);
					ProcessedItemsInStacks += StackCount;

					if (OutChecks->Capacity)
						break;
					++Idx;

					REX::DEBUG("{}", std::string(70, '-'));
				}

				
				if (OutChecks->Capacity)
					OutChecks->InventoryStackId.emplace(Idx);
				else if (const auto NonStackedCount {  Candidate.GetCount() - ProcessedItemsInStacks }; NonStackedCount > 0)
				{
					// The above count represents the items that are not part of the stacks,
					// i.e., have no extradata. So, they are essentially the same as singleton items.
					REX::DEBUG("Ex-stack inventory items | Count: {}", NonStackedCount);
					OutChecks->Capacity = CompareSoulValues(BaseSoulLevel, BaseSoulCapacity, TargetSoulLevel, IsAzurasStar);
				}
			}
		}
		else
		{
			OutChecks->Capacity = CompareSoulValues(BaseSoulLevel, BaseSoulCapacity, TargetSoulLevel, IsAzurasStar);
		}

		return OutChecks->Capacity;
	}

	bool SoulSacrificeSorter(const std::pair<Candidate, SoulSacrificeChecks>& First, const std::pair<Candidate, SoulSacrificeChecks>& Second)
	{
		// Special-case Azura's Star - it gets consumed first if the corresponding
		// config is enabled. For some reason, the formID of the default form is set to zero.
		if (!Config::PrioritizeAzurasStar.GetValue())
			return First.first.SoulGem->cSoulCapacity < Second.first.SoulGem->cSoulCapacity;
		else if (First.first.SoulGem->IsAzurasStar())
			return true;
		else if (Second.first.SoulGem->IsAzurasStar())
			return false; 
		else
			return First.first.SoulGem->cSoulCapacity < Second.first.SoulGem->cSoulCapacity;
	}
}

FinalCandidates SelectBestSoulGem(InventoryChanges* Changes, TESObjectREFR* DeadActor)
{
	using ShortlistT = std::vector<std::pair<Candidate, SoulSacrificeChecks>>;

	const auto TargetSoulLevel{
		DeadActor->data.pObjectReference->cFormType == FormType::CREA_ID ? GetCreatureSoulLevel(DeadActor->data.pObjectReference) : SOUL_LEVEL::SOUL_GRAND
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

	REX::DEBUG("Enumerated {} candidates", MainBucket.size());

	// Check and separate candidates by separating into two more shortlists.
	for (const auto& Itr : MainBucket)
	{
		REX::DEBUG("{}", std::string(70, '='));
		REX::DEBUG("Checking candidate '{}' ({:08X}) | HasInventory: {}", Itr.first->cFullName.pString, Itr.first->iFormID, Itr.second.HasInventory());

		SoulSacrificeChecks NoChecksAndBalances;

		if (!CheckSentience(Itr.second, SentientSoul, &NoChecksAndBalances))
		{
			REX::DEBUG("{}", std::string(70, '='));
			continue;
		}

		if (!CheckCapacity(Itr.second, TargetSoulLevel, &NoChecksAndBalances))
		{
			REX::DEBUG("{}", std::string(70, '='));
			continue;
		}

		if (Itr.second.HasInventory())
			Inventory.emplace_back(Itr.second, NoChecksAndBalances);
		else
			BaseContainerOnly.emplace_back(Itr.second, NoChecksAndBalances);

		REX::DEBUG("{}", std::string(70, '='));
	}

	FinalCandidates Out;

	REX::DEBUG("Shortlists: BaseContainerOnly [{}] | Inventory [{}] ", BaseContainerOnly.size(), Inventory.size());

	// Sort the shortlists such that the smallest (in capacity/value) candidate gets picked.
	// Candidates with inventory changes are given priority.
	if (!Inventory.empty())
	{
		std::sort(Inventory.begin(), Inventory.end(), SoulSacrificeSorter);
		auto& First{ Inventory.front() };

		// We need to set the inventory stack's soul level to none, other the
		// subsequent call to ItemChange::SetSoul will just skip over the stack.
		if (First.second.InventoryStackId != std::nullopt)
		{
			auto xList{ *First.first.InventoryChange->pExtraObjectList->At(*First.second.InventoryStackId) };
			auto xSoul{ reinterpret_cast<ExtraSoul*>(xList->GetExtraData(EXTRA_DATA_TYPE::EXTRA_SOUL)) };
			if (xSoul != nullptr)
				xSoul->cSoul = SOUL_LEVEL::SOUL_NONE;
		}

		Out.Inventory = First.first.InventoryChange;

		REX::INFO("\tFinal Candidate: Inventory '{}' ({:08X}) ", First.first.SoulGem->cFullName.pString, First.first.SoulGem->iFormID);
	}
	else if (!BaseContainerOnly.empty())
	{
		std::sort(BaseContainerOnly.begin(), BaseContainerOnly.end(), SoulSacrificeSorter);
		auto& First{ BaseContainerOnly.front() };
		Out.BaseContainer = First.first.SoulGem;

		REX::INFO("\tFinal Candidate: BaseContainerOnly '{}' ({:08X}) ", First.first.SoulGem->cFullName.pString, First.first.SoulGem->iFormID);
	}
	else
		REX::INFO("\tNo usable soulgem");

	return Out;
}
