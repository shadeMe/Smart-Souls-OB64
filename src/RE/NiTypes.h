#pragma once

// C
struct NiPoint3
{
    float x, y, z;
};
static_assert(sizeof(NiPoint3) == 0xC);

// 8
template <typename T>
struct NiPointer
{
    T *m_pObject; // 00
};
static_assert(sizeof(NiPointer<void>) == 0x8);