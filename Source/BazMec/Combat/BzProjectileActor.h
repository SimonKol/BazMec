// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BzProjectileActor.generated.h"

UCLASS()
class BAZMEC_API ABzProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:

	ABzProjectileActor();

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnStarted(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnImpact(const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnFinished();

	UFUNCTION(BlueprintCallable, Category=Bz)
	void ConfirmRelease();

	void Start(const FVector& Location, const FRotator& Rotation);
	bool IsActive() const { return bIsActive; }
	void OnImpact(const FHitResult& Hit);
	void Finish();

	// Release this actor for re-use immediately on impact or reaching max age. If false, must be released by calling ConfirmRelease
	UPROPERTY(EditDefaultsOnly, Category=Bz)
	bool bReleaseOnFinish = true;

private:

	bool bIsActive = true;
};
