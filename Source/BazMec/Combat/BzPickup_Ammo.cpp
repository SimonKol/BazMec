// (c) Simon Kolciter


#include "BzPickup_Ammo.h"

#include "BzWeaponComponent.h"
#include "BzWeapon.h"

bool ABzPickup_Ammo::CanPickup_Implementation(AActor* User) const
{
	if (UBzWeaponComponent* WeaponComp = UBzWeaponComponent::Get(User))
	{
		return WeaponComp->FindReloadCandidate(ProjectileType) != nullptr;
	}

	return false;
}

void ABzPickup_Ammo::DoPickup_Implementation(AActor* User)
{
	if (UBzWeaponComponent* WeaponComp = UBzWeaponComponent::Get(User))
	{
		if (ABzWeapon* Weapon = WeaponComp->FindReloadCandidate(ProjectileType))
		{
			Weapon->AddAmmo(Amount);
		}
	}
}
