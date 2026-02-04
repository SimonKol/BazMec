// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BzBotCharacter.generated.h"

#define BZ_BOTS_DIAG !UE_BUILD_SHIPPING

class ABzBotController;
class UBzHealthComponent;

UCLASS()
class BAZMEC_API ABzBotCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABzBotCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Bz)
	TObjectPtr<class USceneComponent> AimPoint;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBzHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Bz)
	TObjectPtr<class UBzWeaponComponent> WeaponComponent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBzBotKilledDelegate, ABzBotCharacter*, Sender);
	FBzBotKilledDelegate OnKilled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnHealthChanged(UBzHealthComponent* Sender);

	void SetWantsToFire(bool bValue);

	UPROPERTY()
	TObjectPtr<ABzBotController> BotController;

	bool bWantsToFire = false;
	float NextFireChangeIn = -1;
};
