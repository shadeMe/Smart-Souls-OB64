#include "BaseExtraList.h"

BSExtraData* BaseExtraList::GetExtraData(std::uint8_t Type)
{
	using func_t = decltype(&BaseExtraList::GetExtraData);
	static REL::Relocation<func_t> func{ REL::ID(407219) };
	return func(this, Type);
}
