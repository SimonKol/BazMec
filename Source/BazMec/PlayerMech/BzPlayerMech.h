// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/BzAbilityTypes.h"
#include "Math/BzMathTypes.h"
#include "BzPlayerMech.generated.h"

class USceneComponent;
class ABzWeapon;
class UBzAbility;

UCLASS(PrioritizeCategories=("Bz"))
class BAZMEC_API ABzPlayerMech : public ACharacter
{
	GENERATED_BODY()

public:

	ABzPlayerMech();

	UFUNCTION(BlueprintCallable, Category=Bz)
	ABzWeapon* GetPrimaryWeapon() const { return PrimaryWeapon;}

	UFUNCTION(BlueprintCallable, Category=Bz)
	ABzWeapon* GetSecondaryWeapon() const { return SecondaryWeapon;}

	UFUNCTION(BlueprintCallable, Category=Bz)
	void TryJump();

	UFUNCTION(BlueprintCallable, Category=Bz)
	void TrySpeedBoost();
	
	UFUNCTION(BlueprintPure, Category=Bz)
	const FBzAbilityHandle& GetJumpAbility() const { return JumpAbilityHandle; }

	UFUNCTION(BlueprintPure, Category=Bz)
	const FBzAbilityHandle& GetSpeedBoostAbility() const { return SpeedAbilityHandle; }

	UFUNCTION(BlueprintPure, Category=Bz)
	const FBzAbilityHandle& GerPrimaryAbility() const { return PrimaryAbilityHandle; }

	UFUNCTION(BlueprintPure, Category=Bz)
	const FBzAbilityHandle& GetSecondaryAbility() const { return SecondaryAbilityHandle; }

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnWeaponFired();

	void SetSpeedBoostDuration(float Duration);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> AimPointParent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> AimPoint;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBzHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBzWeaponComponent> WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBzEnergyComponent> EnergyComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBzAbilityComponent> AbilityComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBzDamageDealerComponent> DamageDealerComponent;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	TObjectPtr<const class UBzInputConfigAsset> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta=(ShowTreeView=true))
	TSubclassOf<ABzWeapon> PrimaryWeaponType;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<ABzWeapon> SecondaryWeaponType;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<UBzAbility> PrimaryAbility;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<UBzAbility> SecondaryAbility;

	UPROPERTY(EditDefaultsOnly, Category=Bz, meta = (ShowTreeView = true))
	TSubclassOf<AActor> AbilityTargetingActor;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float NormalSpeed = 600;

	UPROPERTY(EditDefaultsOnly, Category=Bz)
	float BoostSpeed = 900;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void UpdateHeadBob(float DeltaTime);
	ABzWeapon* SpawnWeapon(const TSubclassOf<ABzWeapon>& WeaponClass, float Offset);

	void MainActionInputEvent(const struct FInputActionInstance& Input);
	void AbilityInputEvent(const FInputActionInstance& Input);

	void UpdateAim(float DeltaTime);
	void UpdateAbilityTargeting();

	bool IsSpeedBoostActive() const { return SpeedBoostCooldown > 0.f; }
	void TryActivateAbility(const FBzAbilityHandle& Handle);

	UPROPERTY()
	TObjectPtr<ABzWeapon> PrimaryWeapon;

	UPROPERTY()
	TObjectPtr<ABzWeapon> SecondaryWeapon;

	FRotator PrimaryWeaponCorrection = FRotator::ZeroRotator;
	FRotator SecondaryWeaponCorrection = FRotator::ZeroRotator;

	UPROPERTY()
	TObjectPtr<AActor> TargetingVisual;

	float HeadBobPhase = 0;
	float HeadBobCoef = 0;
	FBzSpringPointState AimPointSpringState;

	float SpeedBoostCooldown = -1;

	FBzAbilityHandle JumpAbilityHandle;
	FBzAbilityHandle SpeedAbilityHandle;
	FBzAbilityHandle PrimaryAbilityHandle;
	FBzAbilityHandle SecondaryAbilityHandle;

	FCollisionQueryParams CachedQueryParams;
	FHitResult CurrentAimHit;
};
