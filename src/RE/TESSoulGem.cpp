#include "TESSoulGem.h"

bool TESSoulGem::IsAzurasStar() const
{
	return this == AzurasStar.get() || iFormID == 0x193 || (iFormID & 0x00FFFFFF) == 0x193;
}

bool TESSoulGem::IsEmptyBlackSoulGem() const
{
	return this == BlackSoulGem.get() || iFormID == 0x192 || (iFormID & 0x00FFFFFF) == 0x192;
}

std::uint32_t TESSoulGem::GetSoulLevelValue(const SOUL_LEVEL SoulLevel)
{
	using func_t = decltype(&TESSoulGem::GetSoulLevelValue);
	static REL::Relocation<func_t> func{ REL::ID(417506) };
	return func(SoulLevel);
}
