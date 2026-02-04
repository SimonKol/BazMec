// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BzWidgetLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzWidgetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category=Bz)
	static bool IsMechLoadingScreenEnabled();

	UFUNCTION(BlueprintPure, Category=Bz)
	static bool FindSlateBrush(FName SlateStyle, FName BrushName, struct FSlateBrush& Brush);

	UFUNCTION(BlueprintPure, Category=Bz)
	static bool GetKeyTextForInputAction(class APlayerController* PlayerController, class UInputAction* Action, FText& KeyText);
};
