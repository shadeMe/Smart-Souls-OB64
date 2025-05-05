
#include "RE/BSSimpleList.h"
#include "RE/TESObjectREFR.h"

class ExtraDataList;
class TESBoundObject;

// 18
class ItemChange
{
public:
	using ExtraObjectListT = BSSimpleList<ExtraDataList*>;

	ExtraObjectListT* pExtraObjectList;              // 00
    std::int32_t iNumber;                            // 08
    std::uint32_t padding0C;                         // 0C
    TESBoundObject *pContainerObj;                   // 10
};
static_assert(sizeof(ItemChange) == 0x18);

// 0x18
class InventoryChanges
{
public:
	using ChangesT = BSSimpleList<ItemChange *>;

    ChangesT* pListofChanges;                   // 00
    TESObjectREFR *pRef;                        // 08
    float fcontainerweight;                     // 10
    float fequippedweight;                      // 14
};
static_assert(sizeof(InventoryChanges) == 0x18);