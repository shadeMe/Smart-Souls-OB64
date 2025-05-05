#include "TESObjectREFR.h"

TESContainer* TESObjectREFR::HasContainer()
{
	using func_t = decltype(&TESObjectREFR::HasContainer);
	static REL::Relocation<func_t> func{ REL::Offset(0x65A9FF0) };
	return func(this);
}
