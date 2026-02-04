// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BzPickup.generated.h"

UCLASS()
class BAZMEC_API ABzPickup : public AActor
{
	GENERATED_BODY()
	
public:

	ABzPickup();

	UFUNCTION(BlueprintCallable, Category=Bz)
	void CheckPickup(AActor* User);

	UFUNCTION(BlueprintNativeEvent, Category=Bz)
	bool CanPickup(AActor* User) const;
	virtual bool CanPickup_Implementation(AActor* User) const { return false; }

	UFUNCTION(BlueprintNativeEvent, Category=Bz)
	void DoPickup(AActor* User);
	virtual void DoPickup_Implementation(AActor* User) {}

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor) { CheckPickup(OtherActor); }

	UPROPERTY()
	TObjectPtr<class USceneComponent> Visual;
};
