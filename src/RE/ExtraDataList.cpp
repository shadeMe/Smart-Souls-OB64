#include "ExtraDataList.h"

SOUL_LEVEL ExtraDataList::GetSoul() const
{
	using func_t = decltype(&ExtraDataList::GetSoul);
	static REL::Relocation<func_t> func{ REL::Offset(0x6641410) };
	return func(this);
}

std::uint16_t ExtraDataList::GetCount() const
{
	using func_t = decltype(&ExtraDataList::GetCount);
	static REL::Relocation<func_t> func{ REL::Offset(0x66403F0) };
	return func(this);
}
