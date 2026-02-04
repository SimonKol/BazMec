// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BzGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BAZMEC_API ABzGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void InitGameState() override;
};
