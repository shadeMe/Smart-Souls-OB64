#pragma once

#include "RE/NiTypes.h"
#include "RE/TESForm.h"
#include "RE/TESChildCell.h"
#include "RE/IVPairableItem.h"
#include "RE/TESMemContextForm.h"
#include "RE/EVUnpairingState.h"
#include "RE/ExtraDataList.h"
#include "RE/TESContainer.h"

class TESBoundObject;
class TESObjectCELL;
class hkRigidBody;
class bhkSimpleShapePhantom;
class NiAVObject;
class TESObjectREFR;
class TESContainer;

// 20
struct OBJ_REFR
{
    TESBoundObject *pObjectReference; // 00
    NiPoint3 Angle;                   // 08
    NiPoint3 Location;                // 14
};
static_assert(sizeof(OBJ_REFR) == 0x20);

// 20
struct FVDoorTeleportationInfo
{
    TESObjectREFR *TeleportingDoor; // 00
    bool bIdFlag;                   // 08
    std::uint8_t pad09[7];          // 09
    TESBoundObject *ObjectToGet;    // 10
    std::int32_t Count;             // 18
    std::uint8_t pad1C[4];          // 1C
};
static_assert(sizeof(FVDoorTeleportationInfo) == 0x20);

// 138
class TESObjectREFR : public TESForm, public TESChildCell, public IVPairableItem, public TESMemContextForm
{
public:
    OBJ_REFR data;                                        // 50
    float fRefScale;                                      // 70
    std::uint8_t pad74[4];                                // 74
    NiPointer<NiAVObject> m_spData3D;                     // 78
    TESObjectCELL *pParentCell;                           // 80
    ExtraDataList m_Extra;                                // 88
    std::uint32_t AsyncFlowHash;                          // A8
    bool bIsOblivionRefLoaded;                            // AC
    bool bIsAltarRefLoaded;                               // AD
    std::uint8_t padAE[2];                                // AE
    hkRigidBody *AltarFakeRigidBody;                      // B0
    bhkSimpleShapePhantom *AltarFakePhantom;              // B8
    bool HasTrapContinuityBeenCheckedInReverse;           // C0
    std::uint8_t padC1[7];                                // C1
    TESObjectREFR *DockedActor;                           // C8
    EVUnpairingState UnpairingState;                      // D0
    std::uint8_t padD4[4];                                // D4
    VPairingEntry *pDroppedBy;                            // D8
    bool bIsBeingTeleported;                              // E0
    std::uint8_t padE1[7];                                // E1
    FVDoorTeleportationInfo PendingDoorTeleportationInfo; // E8
    std::uint32_t LevelCreatureFormID;                    // 108
    std::uint8_t pad10C[4];                               // 10C
    TESContainer ChosenCreatures;                         // 110
    bool bisContainerInit;                                // 130
    std::uint8_t LastLoadFileCompileIndex;                // 131
    std::uint8_t pad132[6];                               // 132

	TESContainer* HasContainer();
};
static_assert(sizeof(TESObjectREFR) == 0x138);
