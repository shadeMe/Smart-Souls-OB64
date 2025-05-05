#include "TESContainer.h"

inline bool TESContainer::ContainsObject(TESBoundObject* obj) const
{
	if (!(contFlag & 1))
		return false;

	for (auto* current = &objectList; current != nullptr; current = current->m_pkNext)
	{
		if (current->m_item->pObj == obj)
		{
			return true;
		}
	}
	return false;
}
