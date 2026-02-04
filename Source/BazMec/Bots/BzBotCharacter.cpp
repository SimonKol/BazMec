// (c) Simon Kolciter


#include "BzBotCharacter.h"

#include "BzBotController.h"
#include "Combat/BzHealthComponent.h"
#include "Combat/BzWeapon.h"
#include "Combat/BzWeaponComponent.h"
#include "Math/BzMathLibrary.h"

#if BZ_BOTS_DIAG
#include "EngineUtils.h"
#endif

// Sets default values
ABzBotCharacter::ABzBotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AimPoint = CreateDefaultSubobject<USceneComponent>("AimPoint");
	AimPoint->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<UBzHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UBzWeaponComponent>("WeaponComponent");
}

void ABzBotCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	BotController = Cast<ABzBotController>(NewController);
}

void ABzBotCharacter::UnPossessed()
{
	Super::UnPossessed();
	BotController = nullptr;
}

void ABzBotCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnStateChanged.AddUniqueDynamic(this, &ABzBotCharacter::OnHealthChanged);
}

void ABzBotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent->IsAlive())
	{
		bool bCanFire = false;
		if (AimPoint)
		{
			if (BotController)
			{
				check(WeaponComponent);
				AActor* AttackTarget = BotController->GetAttackTarget();
				ABzWeapon* Weapon = WeaponComponent->GetFirstWeapon();

				if (AttackTarget && Weapon)
				{
					const FRotator ControlRot = GetControlRotation();

					const FVector TargetPos = AttackTarget->GetActorLocation();
					const FVector TargetSpeed = AttackTarget->GetVelocity();

					const FVector StartLocation = AimPoint->GetComponentLocation();
					const FVector PredictedTarget = UBzMathLibrary::PredictTargetPosition(TargetPos, TargetSpeed, AimPoint->GetComponentLocation(), Weapon->GetCachedProjectileType().Speed);
					//DrawDebugSphere(GetWorld(), LeadingTarget, 15, 0, FColor::Purple, false, 1);

					const FRotator WantedRot = (PredictedTarget - StartLocation).ToOrientationRotator();
					BotController->SetControlRotation(FMath::RInterpTo(ControlRot, WantedRot, DeltaTime, 10));
					bCanFire = true;
				}
			}

			AimPoint->SetWorldRotation(GetControlRotation());
		}

		NextFireChangeIn -= DeltaTime;
		if (NextFireChangeIn <= 0.f)
		{
			SetWantsToFire(!bWantsToFire && bCanFire);
			NextFireChangeIn = bWantsToFire ? FMath::FRandRange(2.f, 4.f) : FMath::FRandRange(1.f, 2.f);
		}
	}
}

void ABzBotCharacter::OnHealthChanged(UBzHealthComponent* Sender)
{
	check(Sender == HealthComponent);

	if (!Sender->IsAlive())
	{
		SetWantsToFire(false);
		SetLifeSpan(10.f);
		OnKilled.Broadcast(this);
	}
}

void ABzBotCharacter::SetWantsToFire(bool bValue)
{
	bWantsToFire = bValue;
	check(WeaponComponent);
	if (ABzWeapon* Weapon = WeaponComponent->GetFirstWeapon())
	{
		Weapon->SetTriggerState(bValue);
	}
}

#if BZ_BOTS_DIAG

FAutoConsoleCommandWithWorld KillAllBotsCmd(TEXT("Bz.Bots.KillAll"), TEXT("Kill all bots"), FConsoleCommandWithWorldDelegate::CreateLambda([](UWorld* World)
	{
		for (TActorIterator<ABzBotCharacter> It(World); It; ++It)
		{
			It->HealthComponent->DealDamage(It->HealthComponent->GetHealth() + 1);
		}
	}));

#endif
