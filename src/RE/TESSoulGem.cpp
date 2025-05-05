#include "TESSoulGem.h"

std::uint32_t TESSoulGem::GetSoulLevelValue(const SOUL_LEVEL SoulLevel)
{
	using func_t = decltype(&TESSoulGem::GetSoulLevelValue);
	static REL::Relocation<func_t> func{ REL::Offset(0x6957860) };
	return func(SoulLevel);
}
