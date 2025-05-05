#pragma once

#include "RE/VPairingEntry.h"

// 18
class IVPairableItem
{
public:
    virtual ~IVPairableItem(); // 00

    bool bIsActuallyPaired;       // 08
    std::uint8_t pad09[7];        // 09
    VPairingEntry *pPairingEntry; // 10
};
static_assert(sizeof(IVPairableItem) == 0x18);