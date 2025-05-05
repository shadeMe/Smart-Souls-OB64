#pragma once

#include "RE/BaseExtraList.h"


// 20
class ExtraDataList : public BaseExtraList
{
public:
    virtual ~ExtraDataList() = default; // 00

	SOUL_LEVEL GetSoul() const;
	std::uint16_t GetCount() const;
};
static_assert(sizeof(ExtraDataList) == 0x20);