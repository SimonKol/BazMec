// (c) Simon Kolciter


#include "BzGameLoopSubsystem.h"

#include "BzEventActor.h"
#include "BzGameLoopConfigAsset.h"
#include "Abilities/BzPickup.h"
#include "Bots/BzBotCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogBzGameLoop, Log, All);

void UBzGameLoopSubsystem::Start()
{
	ConfigAsset.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateWeakLambda(this, [this](const FSoftObjectPath& Path, UObject* LoadedObject)
		{
			Config = Cast<UBzGameLoopConfigAsset>(LoadedObject);
			if (Config)
			{
				UE_LOG(LogBzGameLoop, Log, TEXT("Loaded config asset @ {%s}"), *LoadedObject->GetPathName());
				StartSpawning();
			}
			else
			{
				UE_LOG(LogBzGameLoop, Error, TEXT("Failed to load config asset from {%s}"), *Path.ToString());
			}
		}));
}

static FRotator GetRandomYaw()
{
	return FRotator(0, FMath::RandRange(0., 360.), 0);
}

static float GetRandomSpawnRadius()
{
	return FMath::FRandRange(100.f, 250.f) * 100.f;
}

void UBzGameLoopSubsystem::StartSpawning()
{
	check(Config);

	for (const TSubclassOf<ABzPickup>& PickupType : Config->PickupClasses)
	{
		PickupItems.Add(FPickupInfo { .Type = PickupType});
	}

	const FVector SpawnPos = GetRandomYaw().Vector() * GetRandomSpawnRadius();

	UE_LOG(LogBzGameLoop, Display, TEXT("Created event actor @ %s"), *SpawnPos.ToCompactString());
	EventActor = GetWorld()->SpawnActor<ABzEventActor>(Config->EventActorClass, FTransform(SpawnPos));
	EventActor->OnTriggered.BindUObject(this, &UBzGameLoopSubsystem::OnTriggeredEvent);
	EventActor->SetEnabled(true);
}

void UBzGameLoopSubsystem::SpawnRandomPickup(const FVector& Location)
{
	float TotalWeight = 0;
	for (const FPickupInfo& Info : PickupItems)
	{
		TotalWeight += Info.DynamicWeight;
	}

	const float Hit = FMath::FRand() * TotalWeight;
	float Check = 0;
	UClass* PickupClass = nullptr;

	for (int i = 0; i < PickupItems.Num(); ++i)
	{
		FPickupInfo& Info = PickupItems[i];
		Check += Info.DynamicWeight;

		if (!PickupClass && (Check >= Hit || i == PickupItems.Num()-1))
		{
			PickupClass = Info.Type;
			Info.DynamicWeight = 1;
		}
		else
		{
			Info.DynamicWeight *= 1.4f; // Increase probability of pickups that weren't selected
		}
	}

	GetWorld()->SpawnActor<ABzPickup>(PickupClass, FTransform(Location));
}

void UBzGameLoopSubsystem::OnTriggeredEvent(ABzEventActor* Sender)
{
	SCOPED_NAMED_EVENT(BzGameLoop_OnTriggeredEvent, FColor::Black);

	check(Sender == EventActor);
	EventActor->SetEnabled(false);
	const FVector SpawnCenter = Sender->GetActorLocation() + FVector::UpVector * 300.f;
	const float RotOffset = FMath::FRand() * 360.f;

	int32 Count = FMath::RandRange(5, 9);
	for (int i = 0; i < Count; ++i)
	{
		SCOPED_NAMED_EVENT(BzGameLoop_SpawnBot, FColor::Black);

		const FTransform Transform(GetRandomYaw(), SpawnCenter + FRotator(0, RotOffset + 360.f * i / Count, 0).Vector() * FMath::FRandRange(500.f, 2100.f));
		ABzBotCharacter* Bot = GetWorld()->SpawnActor<ABzBotCharacter>(Config->BotClass, Transform);
		ActiveBots.Add(Bot);
		Bot->OnKilled.AddUniqueDynamic(this, &UBzGameLoopSubsystem::OnBotKilled);
	}
}

void UBzGameLoopSubsystem::RepositionEventActor()
{
	check(EventActor);

	const FVector CurrentLocation = EventActor->GetActorLocation();
	const FVector Dir = CurrentLocation.GetSafeNormal();

	FVector NewLoc = FRotator(0, FMath::FRandRange(90.f, 270.f), 0).RotateVector(Dir) * GetRandomSpawnRadius();
	const FVector Diff = NewLoc - CurrentLocation;

	constexpr float MaxDiff = 300 * 100.f;
	constexpr float MaxDiff2 = MaxDiff * MaxDiff;
	if (Diff.SizeSquared() > MaxDiff2)
	{
		NewLoc = CurrentLocation + Diff.GetSafeNormal() * MaxDiff;
	}

	UE_LOG(LogBzGameLoop, Display, TEXT("Repositioning event actor @ %s"), *NewLoc.ToCompactString());
	EventActor->SetActorLocation(NewLoc);
	EventActor->SetEnabled(true);
}

void UBzGameLoopSubsystem::OnBotKilled(ABzBotCharacter* Bot)
{
	check(Bot);
	ActiveBots.Remove(Bot);

	const float MaxRand = 1 - .7f/FMath::Square(NoPickupCounter+1);
	// Increase probability of spawning a pickup until success, then reset 
	if (FMath::FRand() < MaxRand)
	{
		NoPickupCounter = 0;
		SpawnRandomPickup(Bot->GetActorLocation());
	}
	else
	{
		++NoPickupCounter;
	}

	if (ActiveBots.IsEmpty())
	{
		RepositionEventActor();
	}
}

#if BZ_GAMELOOP_DIAG
void UBzGameLoopSubsystem::TriggerActiveEvent()
{
	if (EventActor)
	{
		OnTriggeredEvent(EventActor);
	}
}

FAutoConsoleCommandWithWorld TriggerActiveEventCmd(TEXT("Bz.GameLoop.TriggerActiveEvent")
	, TEXT("Explicitly trigger the event actor in its current location")
	, FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		if (UBzGameLoopSubsystem* Subsystem = World->GetSubsystem<UBzGameLoopSubsystem>())
		{
			Subsystem->TriggerActiveEvent();
		}
	}));

#endif
