#pragma once

#include "RE/TESBoundObject.h"
#include "RE/BaseFormComponent.h"

// E0
class TESObjectMISC : public TESBoundObject, public TESFullName, public TESModel, public TESIcon, public TESScriptableForm, public TESValueForm, public TESWeightForm
{
public:
	inline static constexpr auto RTTI = RE::RTTI::TESObjectMISC;


    virtual ~TESObjectMISC() = 0; // 00
};
static_assert(sizeof(TESObjectMISC) == 0xE0);
