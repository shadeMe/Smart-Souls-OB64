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
		else
		{
			return ESoulTrapMode::Default;
		}
	}
}
