#pragma once

#include "RE/TESForm.h"

class TESObjectCELL;
class TESObject;

// 20
struct TESObjectList
{
    std::uint32_t lnk_dwCount; // 00
    std::uint8_t pad04[4];     // 04
    TESObject *lnk_lpRoot;     // 08
    TESObject *lnk_lpTail;     // 10
    TESObjectCELL *pOwner;     // 18
};
static_assert(sizeof(TESObjectList) == 0x20);

// 48
class TESObject : public TESForm
{
public:
	inline static constexpr auto RTTI = RE::RTTI::TESObject;

	virtual ~TESObject() = 0;  // 00


    TESObjectList *m_pParentList; // 30
    TESObject *nod_lpPrev;        // 38
    TESObject *nod_lpNext;        // 40
};
static_assert(sizeof(TESObject) == 0x48);
