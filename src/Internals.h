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

	enum class ESelectMode
	{
		BaseContainerOnly,
		InventoryOnly,
		BaseContainerAndInventory
	};

	SoulBucket Select(ESelectMode Mode) const;
	BucketT::const_iterator begin() const;
	BucketT::const_iterator end() const;
};

struct FinalCandidates
{
	TESBoundObject* BaseContainer;
	ItemChange* Inventory;

	FinalCandidates();
};


