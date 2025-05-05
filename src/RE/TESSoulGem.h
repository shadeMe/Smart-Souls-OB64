#pragma once

#include "RE/TESObjectMISC.h"

enum SOUL_LEVEL : std::uint32_t
{  
	SOUL_NONE = 0x0,
	SOUL_PETTY = 0x1,
	SOUL_LESSER = 0x2,
	SOUL_COMMON = 0x3,
	SOUL_GREATER = 0x4,
	SOUL_GRAND = 0x5,
	SOUL_LEVEL_COUNT = 0x6,
};

// E8
class TESSoulGem : public TESObjectMISC
{
public:
	inline static constexpr auto RTTI = RE::RTTI::TESSoulGem;

    virtual ~TESSoulGem() = 0; // 00

    std::uint8_t cCurrentSoul;  // E0
    std::uint8_t cSoulCapacity; // E1
    std::uint8_t padE2[6];      // E2

	static std::uint32_t GetSoulLevelValue(const SOUL_LEVEL SoulLevel);

	inline static REL::Relocation<TESSoulGem*> BlackSoulGem{ REL::Offset(0x9454690) };
	inline static REL::Relocation<TESSoulGem*> AzurasStar{ REL::Offset(0x9454698) };
};
static_assert(sizeof(TESSoulGem) == 0xE8);