// (c) Simon Kolciter


#include "BzProjectileSubsystem.h"

#include "BzCombatSettings.h"
#include "BzDamageDealerComponent.h"
#include "BzDamageLibrary.h"
#include "BzProjectileActor.h"

#define BZ_PROJECTILE_DIAG !UE_BUILD_SHIPPING

static TAutoConsoleVariable<bool> CVarBzProjectileDrawTrace(TEXT("Bz.Projectile.DrawTrace"), false
	, TEXT("Draw projectile motion"), ECVF_Cheat);

static TAutoConsoleVariable<float> CVarBzProjectileDrawThickness(TEXT("Bz.Projectile.DrawThickness"), 0.f
	, TEXT("Thickness of projectile diag draw (0: screenspace)"), ECVF_Cheat);

static TAutoConsoleVariable<bool> CVarBzProjectileDrawImpact(TEXT("Bz.Projectile.DrawImpact"), false
	, TEXT("Draw points or sphere for projectiles with splash damage range"), ECVF_Cheat);

static TAutoConsoleVariable<float> CVarBzProjectileDrawImpactDuration(TEXT("Bz.Projectile.DrawImpactDuration"), 5.f
	, TEXT("Duration for drawn impact points"), ECVF_Cheat);

void UBzProjectileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SCOPED_NAMED_EVENT(BzPreloadProjectileTable, FColor::Black);

	const UBzCombatSettings* Settings = UBzCombatSettings::Get();
	check(Settings);
	if (Settings)
		ProjectileTable = Settings->ProjectileTable.LoadSynchronous();
}

TStatId UBzProjectileSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBzProjectileSubsystem, STATGROUP_Tickables);
}

void UBzProjectileSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SimulateProjectiles(DeltaTime);
}

bool UBzProjectileSubsystem::TryGetProjectileType(FName TypeName, FBzProjectileType& Result)
{
	const UBzCombatSettings* Settings = UBzCombatSettings::Get();
	check(Settings);
	if (Settings && Settings->ProjectileTable.LoadSynchronous())
	{
		if (const FBzProjectileType* Found = Settings->ProjectileTable->FindRow<FBzProjectileType>(TypeName, TEXT("TryGetProjectileType")))
		{
			Result = *Found;
			return true;
		}
	}

	return false;
}

void UBzProjectileSubsystem::NewProjectile(const FBzProjectileType& Projectile, const FVector& Start, const FVector& Dir, AActor* Shooter)
{
	SCOPED_NAMED_EVENT(BzProjectiles_New, FColor::Black);

	FBzProjectileState* State = Projectiles.FindByPredicate([](const FBzProjectileState& InState){ return !InState.bIsActive;});
	if (!State)
	{
		State = &Projectiles.AddDefaulted_GetRef();
	}

	check(State);
	State->bIsActive = true;
	State->Position = Start;
	State->Speed = Projectile.Speed * Dir;
	State->TimeToLive = 20;
	State->Shooter = Shooter;
	State->ProjectileType = Projectile;
	State->ProjectileActor = FindOrMakeActor(Projectile.ProjectileActor);
	State->CachedQueryParams = FCollisionQueryParams();
	State->CachedQueryParams.AddIgnoredActor(Shooter);

	if (State->ProjectileActor)
	{
		State->CachedQueryParams.AddIgnoredActor(State->ProjectileActor);
		State->ProjectileActor->Start(Start, Dir.Rotation());
	}
}

void UBzProjectileSubsystem::SimulateProjectiles(float DeltaTime)
{
	SCOPED_NAMED_EVENT(BzProjectiles_Simulate, FColor::Black);

	UWorld* World = GetWorld();
	const UBzCombatSettings* Settings = UBzCombatSettings::Get();
	check(Settings);
	const ECollisionChannel TraceChannel = Settings->ProjectileChannel;

	for (FBzProjectileState& Projectile : Projectiles)
	{
		if (!Projectile.bIsActive)
			continue;

		const FVector NewPos = Projectile.Position + Projectile.Speed * DeltaTime;

#if BZ_PROJECTILE_DIAG
		if (CVarBzProjectileDrawTrace.GetValueOnGameThread())
		{
			DrawDebugLine(World, Projectile.Position, NewPos, FColor::Red, false, 0.1f, 0, CVarBzProjectileDrawThickness.GetValueOnGameThread());
		}
#endif

		const FBzProjectileType& ProjectileType = Projectile.ProjectileType;

		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, Projectile.Position, NewPos, TraceChannel, Projectile.CachedQueryParams))
		{
			Projectile.bIsActive = false;
			AActor* HitActor = nullptr;

			FBzDamageResult ResultDirect, ResultSplash;

			if (ProjectileType.DirectDamage > SMALL_NUMBER)
			{
				HitActor = Hit.GetActor();
				ResultDirect = UBzDamageLibrary::TryDealDamageSimple(HitActor, ProjectileType.DirectDamage);
			}

			if (ProjectileType.SplashDamage > SMALL_NUMBER)
			{
				// exclude actor that was hit directly but only if direct damage is non-zero
				ResultSplash = UBzDamageLibrary::TryDealSplashDamageSimple(World, Hit.ImpactPoint, ProjectileType.SplashDamageRange, ProjectileType.SplashDamage, TraceChannel, HitActor);
			}

			if (Projectile.ProjectileActor)
			{
				Projectile.ProjectileActor->OnImpact(Hit);
			}

			if (ResultDirect.bHasAny || ResultSplash.bHasAny)
			{
				// TODO: cache somewhere to avoid calling this repeatedly
				if (AActor* Shooter = Projectile.Shooter.Get())
				{
					if (UBzDamageDealerComponent* DamageDealer = Shooter->FindComponentByClass<UBzDamageDealerComponent>())
					{
						DamageDealer->CallDamageDealt();
					}
				}
			}

#if BZ_PROJECTILE_DIAG
			if (CVarBzProjectileDrawImpact.GetValueOnGameThread())
			{
				constexpr float SegmentCoef = TWO_PI / 300.f;
				const float Radius = ProjectileType.SplashDamageRange;
				const int32 Segments = (Radius > SMALL_NUMBER) ? FMath::Max((int32)(Radius * SegmentCoef), 8) : 0;

				DrawDebugSphere(World, Hit.ImpactPoint, FMath::Max(Radius, 2.f), Segments, FColor::Red, false
					, CVarBzProjectileDrawImpactDuration.GetValueOnGameThread(), 0
					, CVarBzProjectileDrawThickness.GetValueOnGameThread());
			}
#endif
		}
		else
		{
			Projectile.TimeToLive -= DeltaTime;
			Projectile.Speed -= FVector::UpVector * ProjectileType.Gravity * 980 * DeltaTime;
			
			if (Projectile.TimeToLive <= 0.f)
				Projectile.bIsActive = false;
			else
			{
				Projectile.Position = NewPos;

				if (Projectile.ProjectileActor)
					Projectile.ProjectileActor->SetActorLocationAndRotation(Projectile.Position, Projectile.Speed.Rotation());
			}
		}

		if (!Projectile.bIsActive)
		{
			if (Projectile.ProjectileActor)
			{
				Projectile.ProjectileActor->Finish();
			}
		}
	}
}

ABzProjectileActor* UBzProjectileSubsystem::FindOrMakeActor(const TSubclassOf<ABzProjectileActor>& ActorClass)
{
	if (!ActorClass)
		return nullptr;

	FBzProjectileActorPool* Pool = PooledActors.Find(ActorClass);
	if (!Pool)
	{
		Pool = &PooledActors.Add(ActorClass);
	}

	if (TObjectPtr<ABzProjectileActor>* FreeActor = Pool->Items.FindByPredicate([](const ABzProjectileActor* Item)
		{
			check(Item);
			return !Item->IsActive();
		}))
	{
		return *FreeActor;
	}

	ABzProjectileActor* NewProj = GetWorld()->SpawnActor<ABzProjectileActor>(ActorClass);
	Pool->Items.Add(NewProj);
	return NewProj;
}
