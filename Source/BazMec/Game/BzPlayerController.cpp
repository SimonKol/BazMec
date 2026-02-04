// (c) Simon Kolciter


#include "BzPlayerController.h"

#include "UI/BzGameScreen.h"

#include "Kismet/GameplayStatics.h"

void ABzPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GameScreenClass)
	{
		GameScreen = Cast<UBzGameScreen>(UUserWidget::CreateWidgetInstance(*this, GameScreenClass, "GameScreen"));
		if (GameScreen)
		{
			GameScreen->AddToViewport();
		}

		SetPaused(false);
	}
}

static const FKey& GetPauseKey()
{
#if WITH_EDITOR
	if (GIsEditor)
	{
		return EKeys::BackSpace;
	}
#endif

	return EKeys::Escape;
}

void ABzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputKeyBinding& Binding = InputComponent->BindKey(GetPauseKey(), IE_Pressed, this, &ABzPlayerController::TogglePause);
	Binding.bExecuteWhenPaused = true;
}

void ABzPlayerController::Unpause()
{
	SetPaused(false);
}

void ABzPlayerController::SetPaused(bool bValue)
{
	bIsPaused = bValue;
	OnPauseStateChanged();
}

void ABzPlayerController::TogglePause()
{
	SetPaused(!bIsPaused);
}

void ABzPlayerController::OnPauseStateChanged()
{
	UGameplayStatics::SetGamePaused(this, bIsPaused);

	if (bIsPaused)
	{
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	bShowMouseCursor = bIsPaused;

	if (GameScreen)
	{
		GameScreen->SetPaused(bIsPaused);
	}
}
