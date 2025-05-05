#pragma once;

class BSExtraData;

// 20
class BaseExtraList
{
public:
    virtual ~BaseExtraList() = 0; // 00

    BSExtraData *pHead;      // 08
    std::uint8_t iFlags[16]; // 10

	BSExtraData* GetExtraData(std::uint8_t Type);
};
static_assert(sizeof(BaseExtraList) == 0x20);