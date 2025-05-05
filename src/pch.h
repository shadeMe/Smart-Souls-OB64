#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <algorithms>

#include <RE/Oblivion.h>
#include <OBSE/OBSE.h>
#include <REL/Trampoline.h>
#include <REX/REX/INI.h>
#include <REX/W32.h>
#include <xbyak/xbyak.h>


#include "RE/TESSoulGem.h"
#include "RE/TESForm.h"
#include "RE/TESObjectREFR.h"
#include "RE/InventoryChanges.h" 
#include "RE/BSExtraData.h" 

using AddressT = REL::Relocation<std::uintptr_t>;

#define NOMINMAX
