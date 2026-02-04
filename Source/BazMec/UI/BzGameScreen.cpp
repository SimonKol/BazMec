// (c) Simon Kolciter


#include "BzGameScreen.h"

void UBzGameScreen::SetPaused(bool bState)
{
	if (bIsPaused != bState)
	{
		bIsPaused = bState;
		BP_OnPaused(bState);
	}
}

void UBzGameScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BP_OnPaused(bIsPaused);
}
