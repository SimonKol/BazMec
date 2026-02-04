// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BzGameScreen.generated.h"

/**
 *
 */
UCLASS()
class BAZMEC_API UBzGameScreen : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetPaused(bool bState);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = Bz)
	void BP_OnPaused(bool bState);

	void NativeOnInitialized() override;

private:

	bool bIsPaused = false;
};
