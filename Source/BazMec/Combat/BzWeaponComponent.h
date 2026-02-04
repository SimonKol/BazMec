// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BzWeaponComponent.generated.h"

class ABzWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAZMEC_API UBzWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBzWeaponComponent();
	
	UFUNCTION(BlueprintPure, Category=Bz, DisplayName="GetBzWeaponComponent")
	static UBzWeaponComponent* Get(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category=Bz)
	ABzWeapon* SpawnWeapon(TSubclassOf<ABzWeapon> WeaponClass);
	ABzWeapon* SpawnWeapon(const TSubclassOf<ABzWeapon>& WeaponClass, TFunction<void(ABzWeapon*)> PostCreationFunc);

	ABzWeapon* FindReloadCandidate(FName ProjectileType);
	ABzWeapon* GetFirstWeapon() const;
	
protected:

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:

	UPROPERTY()
	TArray<TObjectPtr<ABzWeapon>> Weapons;
};
