// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "BzRadarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), PrioritizeCategories = ("Bz"))
class BAZMEC_API UBzRadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBzRadarComponent();

	UFUNCTION(BlueprintCallable, Category=Bz)
	void SetEnabled(bool bValue) { bEnabled = bValue;}
	UFUNCTION(BlueprintPure, Category=Bz)
	bool IsEnabled() const { return bEnabled; }

	UPROPERTY(EditAnywhere, Category=Bz, meta=(Categories=Radar))
	FGameplayTag ObjectTag;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool bEnabled = true;
};
