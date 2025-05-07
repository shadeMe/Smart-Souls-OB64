#pragma once


// 10
template <typename T>
class BSSimpleList
{
public:
    T m_item;                  // 00
    BSSimpleList<T> *m_pkNext; // 08

	inline bool Contains(const T& item) const
	{
		for (auto* current = this; current != nullptr; current = current->m_pkNext)
		{
			if (current->m_item == item)
			{
				return true;
			}
		}
		return false;
	}

	inline std::optional<T> At(const std::size_t idx) {
		std::size_t i {0};
		for (auto* current = this; current != nullptr; current = current->m_pkNext)
		{
			if (i == idx)
				return std::make_optional(current->m_item );
			++i;
		}
		return std::nullopt;
	}

	inline std::size_t Count() const {
		std::size_t i {0};
		for (auto* current = this; current != nullptr; current = current->m_pkNext)
			++i;
		return i;
	}
};
static_assert(sizeof(BSSimpleList<void *>) == 0x10);