#pragma once

namespace Config
{
	enum class ESoulTrapMode
	{
		Default,
		ExactSoulGem,
		UpgradeSoulGem,
	};

	static REX::INI::Bool RestrictBlackSoulGemsToNpcs{ "Patches", "bRestrictBlackSoulGemsToNpcs", false };
	static REX::INI::Bool RestrictNpcsToBlackSoulGems{ "Patches", "bRestrictNpcsToBlackSoulGems", true };
	static REX::INI::Bool UsePrefilledGems{ "Patches", "bUsePrefilledGems", false };
	static REX::INI::Str SoulTrapMode{ "Patches", "sSoulTrapMode", std::string("default") };
	static REX::INI::Bool PrioritizeAzurasStar{ "Patches", "bPrioritizeAzurasStar", false };

	void Load();
	ESoulTrapMode GetSoulTrapMode();
}
