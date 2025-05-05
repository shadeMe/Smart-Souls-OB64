#pragma once

#include "RE/BaseFormComponent.h"
#include "RE/BSSimpleList.h"

class TESBoundObject;

// 10
class ContainerObject
{
public:
    std::int32_t i_count;  // 00
    std::uint8_t pad04[4]; // 04
    TESBoundObject *pObj;  // 08
};
static_assert(sizeof(ContainerObject) == 0x10);

// 20
class TESContainer : public BaseFormComponent
{
public:
    std::uint8_t contFlag;                      // 08
    std::uint8_t pad09[7];                      // 09
    BSSimpleList<ContainerObject *> objectList; // 10

	bool ContainsObject(TESBoundObject* obj) const;
};
static_assert(sizeof(TESContainer) == 0x20);
