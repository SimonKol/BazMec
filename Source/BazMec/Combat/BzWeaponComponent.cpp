// (c) Simon Kolciter


#include "BzWeaponComponent.h"
#include "BzWeapon.h"

UBzWeaponComponent::UBzWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UBzWeaponComponent* UBzWeaponComponent::Get(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<UBzWeaponComponent>() : nullptr;
}

ABzWeapon* UBzWeaponComponent::SpawnWeapon(TSubclassOf<ABzWeapon> WeaponClass)
{
	if (!WeaponClass)
		return nullptr;

	ABzWeapon* Weapon = GetWorld()->SpawnActor<ABzWeapon>(WeaponClass);
	check(Weapon);
	Weapon->SetActorEnableCollision(false);
	Weapons.Add(Weapon);
	return Weapon;
}

ABzWeapon* UBzWeaponComponent::SpawnWeapon(const TSubclassOf<ABzWeapon>& WeaponClass, TFunction<void(ABzWeapon*)> PostCreationFunc)
{
	ABzWeapon* Weapon = SpawnWeapon(WeaponClass);
	if (Weapon)
	{
		PostCreationFunc(Weapon);
	}

	return Weapon;
}

void UBzWeaponComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	for (ABzWeapon* Weapon : Weapons)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
}

ABzWeapon* UBzWeaponComponent::FindReloadCandidate(FName ProjectileType)
{
	for (ABzWeapon* Weapon : Weapons)
	{
		if (Weapon && Weapon->ProjectileType == ProjectileType && Weapon->MagazineSize != 0 && Weapon->GetAmmo() < Weapon->MagazineSize)
		{
			return Weapon;
		}
	}

	return nullptr;
}

ABzWeapon* UBzWeaponComponent::GetFirstWeapon() const
{
	return Weapons.IsEmpty() ? nullptr : Weapons[0];
}
