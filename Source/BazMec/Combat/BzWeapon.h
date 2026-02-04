// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BzProjectileType.h"
#include "BzWeapon.generated.h"

class USceneComponent;

UCLASS(PrioritizeCategories=("Bz"))
class BAZMEC_API ABzWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABzWeapon();

#if WITH_EDITOR
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Bz)
	void FireSingle();

	UFUNCTION(BlueprintCallable, Category = Bz)
	void SetTriggerState(bool bEngaged);

	UFUNCTION(BlueprintPure, Category=Bz)
	float GetHeat() const { return Heat;}

	UFUNCTION(BlueprintPure, Category=Bz)
	float GetReloadProgress() const { return ReloadIn / ReloadDuration;}

	UFUNCTION(BlueprintPure, Category=Bz)
	int GetAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure, Category=Bz)
	bool HasInfiniteAmmo() const { return MagazineSize == 0; }

	UFUNCTION(BlueprintPure, Category=Bz)
	bool HasAmmo() const { return MagazineSize == 0 || CurrentAmmo > 0; }

	// returns ammo left from amount
	UFUNCTION(BlueprintCallable, Category=Bz)
	int32 AddAmmo(int32 Amount);

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnFired(const FVector& Start, const FVector& Speed);

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnOverheat(bool State);

	const FBzProjectileType& GetCachedProjectileType() const { return CachedProjectileType; }

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float ReloadDuration = 1;

	// 0 means no overheating
	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float HeatPerShot = 0;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float CooldownNormal = 0;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float CooldownOverheat = 0;

	// 0 means infinite ammo
	UPROPERTY(EditDefaultsOnly, Category = Bz)
	int32 MagazineSize = 0;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta=(GetOptions="BzCombatSettings.GetProjectileTypes"))
	FName ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category = Bz, meta=(Units="Radians"))
	float AimSpread = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Bz)
	FText DisplayName;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponGenericDelegate, ABzWeapon*, Sender);

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FOnWeaponGenericDelegate OnOverheatChanged;
	
	UPROPERTY(BlueprintAssignable, Category=Bz)
	FOnWeaponGenericDelegate OnAmmoChanged;

	TDelegate<void()> OnFired;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MuzzlePoint;

private:

	bool bIsFiring = false;
	bool bIsOverheaten = false;
	float ReloadIn = 0.f;

	float Heat = 0.f;
	int CurrentAmmo = 0;

	TWeakObjectPtr<class UBzProjectileSubsystem> ProjectileSystemPtr;
	FBzProjectileType CachedProjectileType;
};
