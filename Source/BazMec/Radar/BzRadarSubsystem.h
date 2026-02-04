// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BzRadarSubsystem.generated.h"

class UBzRadarComponent;
/**
 * 
 */
UCLASS()
class BAZMEC_API UBzRadarSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void Register(UBzRadarComponent* Item);
	void Unregister(UBzRadarComponent* Item);

	const TArray<TObjectPtr<UBzRadarComponent>>& GetActiveComponents() const { return ActiveComponents; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FBzRadarComponentDelegate, UBzRadarComponent*);

	FBzRadarComponentDelegate OnItemRegistered;
	FBzRadarComponentDelegate OnItemUnregistered;

private:

	UPROPERTY()
	TArray<TObjectPtr<UBzRadarComponent>> ActiveComponents;
};
