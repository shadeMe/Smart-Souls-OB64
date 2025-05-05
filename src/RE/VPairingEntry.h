#pragma once

class UObject;

// 18
class VPairingEntry
{
public:
    void *OriginalItem;                        // 00
    UObject *HostItem;                         // 08
    bool bIsPaired;                            // 10
    bool bReadyBeDestroyed;                    // 11
    bool bSkipHandlers;                        // 12
    bool bIsHostItemSpawnPending;              // 13
    std::atomic<std::uint32_t> ReferenceCount; // 14
};
static_assert(sizeof(VPairingEntry) == 0x18);
