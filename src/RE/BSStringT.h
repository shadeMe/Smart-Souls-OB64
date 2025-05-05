#pragma once

// 10
template <typename T>
class BSStringT
{
public:
    T *pString;            // 00
    std::uint16_t sLen;    // 08
    std::uint16_t sMaxLen; // 0A
    std::uint8_t pad0C[4]; // 0C
};
static_assert(sizeof(BSStringT<char>) == 0x10);

using BSString = BSStringT<char>;