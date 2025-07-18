#include "Config.h"

namespace Config
{

	void Load()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"OBSE/Plugins/Smart-Souls.ini",
			"OBSE/Plugins/Smart-Souls.ini");
		ini->Load();

		REX::INFO("Loaded INI file");
		REX::INFO("bRestrictBlackSoulGemsToNpcs: {}", RestrictBlackSoulGemsToNpcs.GetValue());
		REX::INFO("bRestrictNpcsToBlackSoulGems: {}", RestrictNpcsToBlackSoulGems.GetValue());
		REX::INFO("bUsePrefilledGems: {}", UsePrefilledGems.GetValue());
		REX::INFO("sSoulTrapMode: {}", SoulTrapMode.GetValue());
		REX::INFO("bPrioritizeAzurasStar: {}", PrioritizeAzurasStar.GetValue());
	}

	ESoulTrapMode GetSoulTrapMode()
	{
		if (SoulTrapMode.GetValue() == "exact")
		{
			return ESoulTrapMode::ExactSoulGem;
		}
		else if (SoulTrapMode.GetValue() == "upgrade")
		{
			return ESoulTrapMode::UpgradeSoulGem;
		}
		else if (SoulTrapMode.GetValue() == "default")
		{
			return ESoulTrapMode::Default;
		}
		else
		{
			REX::WARN("Unknown soul trap mode '{}'; falling back to default mode", SoulTrapMode.GetValue());
			return ESoulTrapMode::Default;
		}
	}
}
