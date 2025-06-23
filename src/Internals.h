#pragma once



struct Candidate
{
	TESSoulGem* SoulGem;
	ContainerObject* BaseContainerItem;
	ItemChange* InventoryChange;

	Candidate(TESSoulGem* SoulGem);

	bool HasInventory() const;
	std::uint16_t GetCount() const;
};

class SoulBucket
{
	using BucketT = std::unordered_map<TESSoulGem*, Candidate>;
	BucketT Store;

public:
	inline SoulBucket() = default;

	void Enroll(TESSoulGem* SoulGem, ContainerObject* BaseContainerItem);
	void Enroll(TESSoulGem* SoulGem, ItemChange* InventoryChange);
	void Enroll(TESSoulGem* SoulGem, ContainerObject* BaseContainerItem, ItemChange* InventoryChange);

	BucketT::const_iterator begin() const;
	BucketT::const_iterator end() const;
	std::size_t size() const;
};

struct FinalCandidates
{
	TESBoundObject* BaseContainer;
	ItemChange* Inventory;

	FinalCandidates();
};

FinalCandidates SelectBestSoulGem(InventoryChanges* Changes, TESObjectREFR* DeadActor);
