#pragma once


enum EXTRA_DATA_TYPE : std::uint32_t
{
	EXTRA_NONE                    = 0x0,
	EXTRA__OLD_CELLCOORD          = 0x1,
	EXTRA_HAVOK                   = 0x2,
	EXTRA_CELL3D                  = 0x3,
	EXTRA_WATERHEIGHT             = 0x4,
	EXTRA_WATERTYPE               = 0x5,
	EXTRA__OLD_LAND               = 0x6,
	EXTRA__OLD_PATHGRID           = 0x7,
	EXTRA_REGIONLIST              = 0x8,
	EXTRA_SEENDATA                = 0x9,
	EXTRA_EDITOR_ID               = 0xA,
	EXTRA_CELLMUSICTYPE           = 0xB,
	EXTRA_CLIMATE                 = 0xC,
	EXTRA_CELL_MID_LOW_PROCESS    = 0xD,
	EXTRA__OLD_CELLLIGHTING       = 0xE,
	EXTRA_CELL_CANOPYSHADOW_MASK  = 0xF,
	EXTRA_CELLDETACHTIME          = 0x10,
	EXTRA_PERSISTENT_CELL         = 0x11,
	EXTRA_SCRIPT                  = 0x12,
	EXTRA_ACTION                  = 0x13,
	EXTRA_STARTINGPOSITION        = 0x14,
	EXTRA_ANIM                    = 0x15,
	EXTRA_BIPED                   = 0x16,
	EXTRA_USEDMARKERS             = 0x17,
	EXTRA_DISTANTDATA             = 0x18,
	EXTRA_RAGDOLLDATA             = 0x19,
	EXTRA_CONTAINER_CHANGES       = 0x1A,
	EXTRA_WORN                    = 0x1B,
	EXTRA_WORN_LEFT               = 0x1C,
	EXTRA__OLD_EDITORLOC          = 0x1D,
	EXTRA_PACKAGESTARTLOC         = 0x1E,
	EXTRA_PACKAGE                 = 0x1F,
	EXTRA_TRESPASS_PACKAGE        = 0x20,
	EXTRA_RUN_ONCE_PACKAGES       = 0x21,
	EXTRA_REFERENCE_POINTER       = 0x22,
	EXTRA_FOLLOWER                = 0x23,
	EXTRA_LEVCREA_MOD             = 0x24,
	EXTRA_GHOST                   = 0x25,
	EXTRA_ORIGINAL_REFERENCE      = 0x26,
	EXTRA_OWNERSHIP               = 0x27,
	EXTRA_GLOBAL                  = 0x28,
	EXTRA_RANK                    = 0x29,
	EXTRA_COUNT                   = 0x2A,
	EXTRA_HEALTH                  = 0x2B,
	EXTRA_USES                    = 0x2C,
	EXTRA_TIMELEFT                = 0x2D,
	EXTRA_CHARGE                  = 0x2E,
	EXTRA_SOUL                    = 0x2F,
	EXTRA_LIGHT                   = 0x30,
	EXTRA_LOCK                    = 0x31,
	EXTRA_TELEPORT                = 0x32,
	EXTRA_MAPMARKER               = 0x33,
	EXTRA_ANIM_SAVE               = 0x34,
	EXTRA_LEVELEDCREATURE         = 0x35,
	EXTRA_LEVELITEM               = 0x36,
	EXTRA_SCALE                   = 0x37,
	EXTRA_SEED                    = 0x38,
	EXTRA_MAGICCASTER             = 0x39,
	EXTRA_MAGICTARGET             = 0x3A,
	EXTRA_MASTERFILECELL          = 0x3B,
	EXTRA_BARTER_GOLD_UNUSED      = 0x3C,
	EXTRA_CRIMEGOLD               = 0x3D,
	EXTRA_OBLIVIONENTRY           = 0x3E,
	EXTRA_ENABLESTATEPARENT       = 0x3F,
	EXTRA_ENABLESTATECHILDREN     = 0x40,
	EXTRA_ITEMDROPPER             = 0x41,
	EXTRA_DROPPEDITEMLIST         = 0x42,
	EXTRA_TELEPORTMARKER          = 0x43,
	EXTRA_MERCHANTCONTAINER       = 0x44,
	EXTRA_TEMPLATEACTORBASE       = 0x45,
	EXTRA_PERSUASIONPERCENTAGE    = 0x46,
	EXTRA_CANNOTWEAR              = 0x47,
	EXTRA_POISON                  = 0x48,
	EXTRA_MAGIC_LIGHT             = 0x49,
	EXTRA_LASTFINISHEDSEQUENCE    = 0x4A,
	EXTRA_SAVEDMOVEMENTDATA       = 0x4B,
	EXTRA_NORTHROTATION           = 0x4C,
	EXTRA_XTARGET                 = 0x4D,
	EXTRA_FRIEND_HIT_LIST         = 0x4E,
	EXTRA_HEAD_TRACK_TARGET       = 0x4F,
	EXTRA_BOUND_ARMOR             = 0x50,
	EXTRA_REFRACTION_PROPERTY     = 0x51,
	EXTRA_INVESTMENT_GOLD         = 0x52,
	EXTRA_STARTINGWORLDORCELL     = 0x53,
	EXTRA_SCRIPT_MODIFIER_NOTUSED = 0x54,
	EXTRA_QUICK_KEY               = 0x55,
	EXTRA_EDITOR_REF3D_DATA       = 0x56,
	EXTRA_EDITOR_REFMOVE_DATA     = 0x57,
	EXTRA_TRAVELHORSE             = 0x58,
	EXTRA_INFO_GENERAL_TOPIC      = 0x59,
	EXTRA_NO_RUMORS               = 0x5A,
	EXTRA_SOUND                   = 0x5B,
	EXTRA_HAGGLE_AMOUNT           = 0x5C,
	EXTRA_ALTAR_BP_VARIANT        = 0x5D,
	EXTRA_ALTAR_ACTOR_GUID        = 0x5E,
	EXTRA_ALTAR_CELL_NAME         = 0x5F,
	EXTRA_ALTARDATA               = 0x60,
	EXTRA_ALTAR_THREATLEVEL       = 0x61,
	EXTRA_ALTAR_BIGWORLD_LEVELEDREGION = 0x62,
	EXTRA_DATATYPECOUNT           = 0x63,
};

// 18
class BSExtraData
{
public:
	virtual ~BSExtraData() = 0;  // 00

	std::uint8_t cEtype;    // 08
	std::uint8_t pad09[7];  // 09
	BSExtraData* pNext;     // 10
};
static_assert(sizeof(BSExtraData) == 0x18);



// 20
class ExtraCount : public BSExtraData
{
public:
	virtual ~ExtraCount() = 0;  // 00

	std::int16_t iCount;    // 18
	std::uint8_t pad1A[6];  // 1A
};

static_assert(sizeof(ExtraCount) == 0x20);


// 20
class ExtraSoul : public BSExtraData
{
public:
	virtual ~ExtraSoul() = 0;  // 00

	std::int8_t cSoul;    // 18
	std::uint8_t pad1A[7];  // 19
};

static_assert(sizeof(ExtraSoul) == 0x20);

