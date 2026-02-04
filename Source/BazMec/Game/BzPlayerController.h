// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BzPlayerController.generated.h"

class UBzGameScreen;

/**
 *
 */
UCLASS()
class BAZMEC_API ABzPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	void BeginPlay() override;
	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = Bz)
	void Unpause();

	UPROPERTY(EditDefaultsOnly, Category = Bz)
	TSubclassOf<UBzGameScreen> GameScreenClass;

private:

	void SetPaused(bool paused);
	void TogglePause();
	void OnPauseStateChanged();

	UPROPERTY()
	TObjectPtr<UBzGameScreen> GameScreen;

	bool bIsPaused = false;
};
