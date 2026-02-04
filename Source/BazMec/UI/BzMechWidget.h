// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BzMechWidget.generated.h"

class ABzPlayerMech;
/**
 * 
 */
UCLASS()
class BAZMEC_API UBzMechWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Bz)
	void SetMech(ABzPlayerMech* Mech);
	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category=Bz)
	void BP_OnSetMech(ABzPlayerMech* Mech);
};
