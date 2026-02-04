// (c) Simon Kolciter


#include "BzCombatSettings.h"
#include "BzProjectileType.h"

#include "Engine/DataTable.h"

const UBzCombatSettings* UBzCombatSettings::Get()
{
	return GetDefault<UBzCombatSettings>();
}

#if WITH_EDITOR
TArray<FName> UBzCombatSettings::GetProjectileTypes()
{
	if (const UBzCombatSettings* Settings = Get())
	{
		if (UDataTable* Table = Settings->ProjectileTable.LoadSynchronous())
		{
			if (Table->GetRowStruct() == FBzProjectileType::StaticStruct())
			{
				return Table->GetRowNames();
			}
		}
	}
	
	return TArray<FName>();
}
#endif
