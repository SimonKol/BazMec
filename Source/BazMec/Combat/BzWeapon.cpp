// (c) Simon Kolciter


#include "BzWeapon.h"
#include "BzProjectileSubsystem.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogBzWeapon, Log, All);

// Sets default values
ABzWeapon::ABzWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>("MuzzlePoint");
	MuzzlePoint->SetupAttachment(Root);
}

#if WITH_EDITOR
EDataValidationResult ABzWeapon::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Parent = Super::IsDataValid(Context);
	if (Parent == EDataValidationResult::Valid)
	{
		if (ReloadDuration < SMALL_NUMBER)
		{
			Context.AddError(INVTEXT("Reload duration must be >= 0"));
			return EDataValidationResult::Invalid;
		}

		if (HeatPerShot >= SMALL_NUMBER)
		{
			if (CooldownNormal < SMALL_NUMBER || CooldownOverheat < SMALL_NUMBER)
			{
				Context.AddError(INVTEXT("Weapon with heat must have cooldowns >= 0"));
				return EDataValidationResult::Invalid;
			}
		}

		if (MagazineSize < 0)
		{
			Context.AddError(INVTEXT("Magazine size must be >= 0"));
			return EDataValidationResult::Invalid;
		}
	}

	return Parent;
}
#endif

void ABzWeapon::FireSingle()
{
	if (UBzProjectileSubsystem* ProjectileSystem = ProjectileSystemPtr.Get())
	{
		FTransform Muzzle = MuzzlePoint->GetComponentTransform();

		const FVector Start = Muzzle.GetLocation();
		const FVector Direction = FMath::VRandCone(Muzzle.GetRotation().Vector(), AimSpread);

		ProjectileSystem->NewProjectile(CachedProjectileType, Start, Direction, GetAttachParentActor());
		BP_OnFired(Start, Direction * CachedProjectileType.Speed);
		OnFired.ExecuteIfBound();
	}

	ReloadIn = ReloadDuration;
	
	if (MagazineSize > 0)
	{
		--CurrentAmmo;
		OnAmmoChanged.Broadcast(this);
	}
	
	Heat += HeatPerShot;
	if (Heat >= 1.f)
	{
		BP_OnOverheat(bIsOverheaten = true);
		OnOverheatChanged.Broadcast(this);
	}
}

void ABzWeapon::SetTriggerState(bool bEngaged)
{
	bIsFiring = bEngaged;
	if (bEngaged && ReloadIn <= 0.f && !bIsOverheaten && HasAmmo())
	{
		FireSingle();
	}
}

int32 ABzWeapon::AddAmmo(int32 Amount)
{
	CurrentAmmo += Amount;
	int32 Remaining = CurrentAmmo - MagazineSize;
	CurrentAmmo = FMath::Min(CurrentAmmo, MagazineSize);
	OnAmmoChanged.Broadcast(this);
	return Remaining;
}

void ABzWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileSystemPtr = GetWorld()->GetSubsystem<UBzProjectileSubsystem>();
	check(ProjectileSystemPtr.IsValid());
	if (!ProjectileSystemPtr->TryGetProjectileType(ProjectileType, CachedProjectileType))
	{
		UE_LOG(LogBzWeapon, Error, TEXT("Invalid projectile type (%s) for weapon {%s}"), *ProjectileType.ToString(), *GetPathName());
	}

	CurrentAmmo = MagazineSize;
	ReloadIn = ReloadDuration;
	BP_OnOverheat(false);
	OnOverheatChanged.Broadcast(this);
	OnAmmoChanged.Broadcast(this);
}

// Called every frame
void ABzWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Heat -= DeltaTime * (bIsOverheaten ? CooldownOverheat : CooldownNormal);
	if (bIsOverheaten && Heat <= 0.f)
	{
		BP_OnOverheat(bIsOverheaten = false);
		OnOverheatChanged.Broadcast(this);
	}

	Heat = FMath::Max(Heat, 0.f);

	if (ReloadIn > 0.f && !bIsOverheaten && HasAmmo())
	{
		ReloadIn -= DeltaTime;
		if (bIsFiring && ReloadIn <= 0.f)
		{
			float CurrentReload = ReloadIn;
			FireSingle();

			// sync firerate with tickrate, but only allow 1 shot per tick
			ReloadIn = FMath::Max(ReloadIn - FMath::Abs(CurrentReload), 1e-5);
		}
	}
}
