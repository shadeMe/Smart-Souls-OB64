
#pragma once

#include "RE/BSSimpleList.h"
#include "RE/BaseFormComponent.h"

class ExportTESForm;
class TESFile;

enum FormType : std::uint8_t
{
	NO_FORM = 0x0,
	TES4_ID = 0x1,
	GRUP_ID = 0x2,
	GMST_ID = 0x3,
	GLOB_ID = 0x4,
	CLAS_ID = 0x5,
	FACT_ID = 0x6,
	HAIR_ID = 0x7,
	EYES_ID = 0x8,
	RACE_ID = 0x9,
	SOUN_ID = 0xA,
	SKIL_ID = 0xB,
	MGEF_ID = 0xC,
	SCPT_ID = 0xD,
	LTEX_ID = 0xE,
	ENCH_ID = 0xF,
	SPEL_ID = 0x10,
	BSGN_ID = 0x11,
	ACTI_ID = 0x12,
	APPA_ID = 0x13,
	ARMO_ID = 0x14,
	BOOK_ID = 0x15,
	CLOT_ID = 0x16,
	CONT_ID = 0x17,
	DOOR_ID = 0x18,
	INGR_ID = 0x19,
	LIGH_ID = 0x1A,
	MISC_ID = 0x1B,
	STAT_ID = 0x1C,
	GRAS_ID = 0x1D,
	TREE_ID = 0x1E,
	FLOR_ID = 0x1F,
	FURN_ID = 0x20,
	WEAP_ID = 0x21,
	AMMO_ID = 0x22,
	NPC__ID = 0x23,
	CREA_ID = 0x24,
	LVLC_ID = 0x25,
	SLGM_ID = 0x26,
	KEYM_ID = 0x27,
	ALCH_ID = 0x28,
	SBSP_ID = 0x29,
	SGST_ID = 0x2A,
	LVLI_ID = 0x2B,
	SNDG_ID = 0x2C,
	WTHR_ID = 0x2D,
	CLMT_ID = 0x2E,
	REGN_ID = 0x2F,
	CELL_ID = 0x30,
	REFR_ID = 0x31,
	ACHR_ID = 0x32,
	ACRE_ID = 0x33,
	PGRD_ID = 0x34,
	WRLD_ID = 0x35,
	LAND_ID = 0x36,
	TLOD_ID = 0x37,
	ROAD_ID = 0x38,
	DIAL_ID = 0x39,
	INFO_ID = 0x3A,
	QUST_ID = 0x3B,
	IDLE_ID = 0x3C,
	PACK_ID = 0x3D,
	CSTY_ID = 0x3E,
	LSCR_ID = 0x3F,
	LVSP_ID = 0x40,
	ANIO_ID = 0x41,
	WATR_ID = 0x42,
	EFSH_ID = 0x43,
	TOFT_ID = 0x44,
	LVRG_ID = 0x45,
	FORM_ID_COUNT = 0x46,
};

// 30
class TESForm : public BaseFormComponent
{
public:
	inline static constexpr auto RTTI = RE::RTTI::TESForm;


	virtual ~TESForm() = 0;  // 00

    // members
    std::uint8_t cFormType;               // 08
    std::uint8_t pad09[3];                // 09
    std::uint32_t iFormFlags;             // 0C
    std::uint32_t iFormID;                // 10
    std::uint8_t pad14[4];                // 14
    BSSimpleList<TESFile *> pSourceFiles; // 18
    ExportTESForm *pExportForm;           // 28
};
static_assert(sizeof(TESForm) == 0x30);