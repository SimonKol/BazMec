// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BzGameLoopSubsystem.generated.h"

#define BZ_GAMELOOP_DIAG !UE_BUILD_SHIPPING

class UBzGameLoopConfigAsset;
class ABzEventActor;
class ABzBotCharacter;
/**
 * 
 */
UCLASS(Config = BzGame, DefaultConfig)
class BAZMEC_API UBzGameLoopSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	void Start();

	UFUNCTION(BlueprintPure, Category=Bz)
	UBzGameLoopConfigAsset* GetConfig() const { return Config; }

#if BZ_GAMELOOP_DIAG
	void TriggerActiveEvent();
#endif

	UPROPERTY(Config)
	TSoftObjectPtr<UBzGameLoopConfigAsset> ConfigAsset;

private:

	void StartSpawning();

	void SpawnRandomPickup(const FVector& Location);
	void OnTriggeredEvent(ABzEventActor* Sender);
	void RepositionEventActor();

	UFUNCTION()
	void OnBotKilled(ABzBotCharacter* Bot);

	UPROPERTY()
	TObjectPtr<UBzGameLoopConfigAsset> Config;

	UPROPERTY()
	TObjectPtr<ABzEventActor> EventActor;

	UPROPERTY()
	TArray<TObjectPtr<ABzBotCharacter>> ActiveBots;

	struct FPickupInfo
	{
		TSubclassOf<class ABzPickup> Type;
		float DynamicWeight = 1;
	};

	TArray<FPickupInfo> PickupItems;
	int NoPickupCounter = 0;
};
