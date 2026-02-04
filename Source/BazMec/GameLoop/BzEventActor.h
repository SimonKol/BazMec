// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BzEventActor.generated.h"



UCLASS()
class BAZMEC_API ABzEventActor : public AActor
{
	GENERATED_BODY()
	
public:

	ABzEventActor();

	UFUNCTION(BlueprintCallable, Category=Bz)
	void Trigger();

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnSetEnabled(bool bValue);

	void SetEnabled(bool bValue);

	DECLARE_DELEGATE_OneParam(FEventTriggeredDelegate, ABzEventActor*);
	FEventTriggeredDelegate OnTriggered;

protected:

	virtual void BeginPlay() override;

private:

	bool bEnabled = false;
};
