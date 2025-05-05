#pragma once

#include "RE/TESObject.h"

// 48
class TESBoundObject : public TESObject
{
public:
	inline static constexpr auto RTTI = RE::RTTI::TESBoundObject;

	virtual ~TESBoundObject() = 0;  // 00
};
static_assert(sizeof(TESBoundObject) == 0x48);