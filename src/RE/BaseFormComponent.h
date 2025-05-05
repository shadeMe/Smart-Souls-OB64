#pragma once

#include "RE/BSSimpleList.h"
#include "RE/BSStringT.h"

class BSFileEntry;
class Script;

// 8
class BaseFormComponent
{
public:
	inline static constexpr auto RTTI = RE::RTTI::BaseFormComponent;


    virtual ~BaseFormComponent() = 0; // 00
};
static_assert(sizeof(BaseFormComponent) == 0x8);

// 18
class TESFullName : public BaseFormComponent
{
public:
    virtual ~TESFullName() = 0; // 00

    BSString cFullName; // 08
};

// 10
struct TESTextureList
{
    std::uint8_t cTextureCount;        // 00
    std::uint8_t pad01[7];             // 01
    BSFileEntry **pTextureOffsetArray; // 08
};
static_assert(sizeof(TESTextureList) == 0x10);

// 30
class TESModel : public BaseFormComponent
{
public:
	virtual ~TESModel() = 0;  // 00

    BSString cModel;            // 08
    float fBoundSize;           // 18
    std::uint8_t pad1C[4];      // 1C
    TESTextureList TextureList; // 20
};
static_assert(sizeof(TESModel) == 0x30);

// 18
class TESTexture : public BaseFormComponent
{
public:
	virtual ~TESTexture() = 0;  // 00

    BSString TextureName; // 08
};
static_assert(sizeof(TESTexture) == 0x18);

// 18
class TESIcon : public TESTexture
{
public:
	virtual ~TESIcon() = 0;  // 00
};
static_assert(sizeof(TESIcon) == 0x18);

// 18
class TESScriptableForm : public BaseFormComponent
{
public:
	virtual ~TESScriptableForm() = 0;  // 00

    Script *pFormScript;   // 08
    bool bIsInitialized;   // 10
    std::uint8_t pad11[7]; // 11
};
static_assert(sizeof(TESScriptableForm) == 0x18);

// 10
class TESWeightForm : public BaseFormComponent
{
public:
	virtual ~TESWeightForm() = 0;  // 00

    float fWeight;         // 08
    std::uint8_t pad0C[4]; // 0C
};
static_assert(sizeof(TESWeightForm) == 0x10);

// 10
class TESValueForm : public BaseFormComponent
{
public:
	virtual ~TESValueForm() = 0;  // 00

    std::int32_t iValue;   // 08
    std::uint8_t pad0C[4]; // 0C
};
static_assert(sizeof(TESValueForm) == 0x10);