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
	static REX::INI::Bool UsePrefilledGems{ "Patches", "bUsePrefilledGems", false };
	static REX::INI::Str SoulTrapMode{ "Patches", "sSoulTrapMode", std::string("default") };

	void Load();
	ESoulTrapMode GetSoulTrapMode();
}
