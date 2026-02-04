// (c) Simon Kolciter

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BzProjectileType.h"
#include "BzProjectileSubsystem.generated.h"

class AActor;
class ABzProjectileActor;

struct FBzProjectileState
{
	bool bIsActive = false;
	float TimeToLive = 0;
	FVector Position = FVector::ZeroVector;
	FVector Speed = FVector::ForwardVector;
	TWeakObjectPtr<AActor> Shooter;
	FBzProjectileType ProjectileType;
	TObjectPtr<ABzProjectileActor> ProjectileActor;
	FCollisionQueryParams CachedQueryParams;
};

USTRUCT()
struct FBzProjectileActorPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<ABzProjectileActor>> Items;
};

/**
 * 
 */
UCLASS()
class BAZMEC_API UBzProjectileSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Bz)
	bool TryGetProjectileType(UPARAM(Meta=(GetOptions="BzCombatSettings.GetProjectileTypes")) FName TypeName, FBzProjectileType& Result);

	UFUNCTION(BlueprintCallable, Category=Bz)
	void NewProjectile(const FBzProjectileType& Projectile, const FVector& Start, const FVector& Dir, AActor* Shooter);

private:

	void SimulateProjectiles(float DeltaTime);
	ABzProjectileActor* FindOrMakeActor(const TSubclassOf<ABzProjectileActor>& ActorClass);
	TArray<FBzProjectileState> Projectiles;

	UPROPERTY()
	TMap<TObjectPtr<UClass>, FBzProjectileActorPool> PooledActors;

	UPROPERTY()
	TObjectPtr<UDataTable> ProjectileTable;
};
