// (c) Simon Kolciter


#include "BzWidgetLibrary.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Styling/SlateStyleRegistry.h"

TAutoConsoleVariable<bool> CVarBzMechUILoadingScreenEnabled(TEXT("Bz.MechUI.LoadingScreenEnabled"), true, TEXT("If false, mech UI is visible immediately"), ECVF_Cheat);

bool UBzWidgetLibrary::IsMechLoadingScreenEnabled()
{
#if !UE_BUILD_SHIPPING
	return CVarBzMechUILoadingScreenEnabled.GetValueOnGameThread();
#else
	return true;
#endif
}

bool UBzWidgetLibrary::FindSlateBrush(FName SlateStyle, FName BrushName, FSlateBrush& Brush)
{
	if (const ISlateStyle* Style = FSlateStyleRegistry::FindSlateStyle(SlateStyle))
	{
		if (const FSlateBrush* Found = Style->GetBrush(BrushName))
		{
			Brush = *Found;
			return true;
		}
	}
	return false;
}

bool UBzWidgetLibrary::GetKeyTextForInputAction(class APlayerController* PlayerController, class UInputAction* Action, FText& KeyText)
{
	if (PlayerController)
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (const UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(Action);
				if (!Keys.IsEmpty())
				{
					KeyText = Keys[0].GetDisplayName(false);
					return true;
				}
			}
		}
	}

	return false;
}
