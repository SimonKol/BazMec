// (c) Simon Kolciter


#include "BzGameMode.h"
#include "GameLoop/BzGameLoopSubsystem.h"

void ABzGameMode::InitGameState()
{
	Super::InitGameState();

	GetWorld()->GetSubsystem<UBzGameLoopSubsystem>()->Start();
}
