// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BzDamageDealerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAZMEC_API UBzDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category=Bz, DisplayName="GetBzDamageDealerComponent")
	static UBzDamageDealerComponent* Get(AActor* Actor);

	void CallDamageDealt();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamageDealtDelegate);

	UPROPERTY(BlueprintAssignable, Category=Bz)
	FDamageDealtDelegate OnDamageDealt;
};
