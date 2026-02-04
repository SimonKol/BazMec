// (c) Simon Kolciter


#include "BzPlayerMech.h"

#include "BzAbility_Jump.h"
#include "BzAbility_SpeedBoost.h"

#include "Abilities/BzAbilityComponent.h"
#include "Abilities/BzEnergyComponent.h"
#include "Combat/BzCombatSettings.h"
#include "Combat/BzDamageDealerComponent.h"
#include "Combat/BzHealthComponent.h"
#include "Combat/BzWeapon.h"
#include "Combat/BzWeaponComponent.h"
#include "Input/BzInputConfigAsset.h"
#include "Math/BzMathLibrary.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

ABzPlayerMech::ABzPlayerMech()
{
	PrimaryActorTick.bCanEverTick = true;

	AimPointParent = CreateDefaultSubobject<USceneComponent>("AimPointParent");
	AimPointParent->SetupAttachment(GetRootComponent());
	AimPoint = CreateDefaultSubobject<USceneComponent>("AimPoint");
	AimPoint->SetupAttachment(AimPointParent);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(AimPointParent);

	HealthComponent = CreateDefaultSubobject<UBzHealthComponent>("HealtHComponent");
	EnergyComponent = CreateDefaultSubobject<UBzEnergyComponent>("EnergyComponent");
	AbilityComponent = CreateDefaultSubobject<UBzAbilityComponent>("AbilityComponent");
	WeaponComponent = CreateDefaultSubobject<UBzWeaponComponent>("WeaponCpomponent");
	DamageDealerComponent = CreateDefaultSubobject<UBzDamageDealerComponent>("DamageDealerComponent");
}

void ABzPlayerMech::BeginPlay()
{
	Super::BeginPlay();

	JumpAbilityHandle = AbilityComponent->AddAbility<UBzAbility_Jump>();
	SpeedAbilityHandle = AbilityComponent->AddAbility<UBzAbility_SpeedBoost>();

	PrimaryAbilityHandle = AbilityComponent->AddAbility(PrimaryAbility);
	SecondaryAbilityHandle = AbilityComponent->AddAbility(SecondaryAbility);

	PrimaryWeapon = SpawnWeapon(PrimaryWeaponType, -100.f);
	SecondaryWeapon = SpawnWeapon(SecondaryWeaponType, 100.f);

	CachedQueryParams.AddIgnoredActor(this);

	if (AbilityTargetingActor)
	{
		TargetingVisual = GetWorld()->SpawnActor(AbilityTargetingActor);
		TargetingVisual->SetActorHiddenInGame(true);
	}
}

void ABzPlayerMech::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SCOPED_NAMED_EVENT(BzPlayerMech_Tick, FColor::Black);

	GetCharacterMovement()->MaxWalkSpeed = IsSpeedBoostActive() ? BoostSpeed : NormalSpeed;
	AimPointParent->SetWorldRotation(GetControlRotation());
	UpdateHeadBob(DeltaTime);
	UpdateAim(DeltaTime);
	UpdateAbilityTargeting();

	if (IsSpeedBoostActive())
	{
		SpeedBoostCooldown -= DeltaTime;
	}
}

void ABzPlayerMech::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInput);
	if (!ensureMsgf(InputConfig, TEXT("InputConfig not defined (%s)"), *GetPathName()))
	{
		return;
	}

	for (const UInputAction* Action : { InputConfig->PrimaryAction, InputConfig->SecondaryAction })
	{
		for (ETriggerEvent Trigger : {ETriggerEvent::Started, ETriggerEvent::Completed, ETriggerEvent::Canceled})
		{
			EnhancedInput->BindAction(Action, Trigger, this, &ABzPlayerMech::MainActionInputEvent);
		}
	}

	for (const UInputAction* Action : { InputConfig->PrimaryAbility, InputConfig->SecondaryAbility })
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ABzPlayerMech::AbilityInputEvent);
	}
}

void ABzPlayerMech::MainActionInputEvent(const FInputActionInstance& Input)
{
	check(InputConfig);
	const bool bValue = !!(Input.GetTriggerEvent() & ETriggerEvent::Started) 
		&& !(Input.GetTriggerEvent() & (ETriggerEvent::Completed | ETriggerEvent::Canceled));
	const bool bPrimary = InputConfig->PrimaryAction == Input.GetSourceAction();

	if (AbilityComponent->IsTargetingAbility() && bValue)
	{
		if (bPrimary)
		{
			if (CurrentAimHit.IsValidBlockingHit())
			{
				AbilityComponent->ConfirmTarget(CurrentAimHit);
			}
		}
		else
		{
			AbilityComponent->StopTargeting();
		}
	}
	else
	{
		ABzWeapon* Weapon = bPrimary ? PrimaryWeapon : SecondaryWeapon;
		if (Weapon)
		{
			Weapon->SetTriggerState(bValue);
		}
	}
}

void ABzPlayerMech::AbilityInputEvent(const FInputActionInstance& Input)
{
	check(InputConfig);
	FBzAbilityHandle Handle = (Input.GetSourceAction() == InputConfig->PrimaryAbility) ? PrimaryAbilityHandle : SecondaryAbilityHandle;
	TryActivateAbility(Handle);
}

namespace 
{
	constexpr float AimPointRayDistance = 1000 * 100;
	constexpr float WeaponRotCorrectionSpeed = 40.f;
	constexpr float MaxWeaponAngle = FMath::DegreesToRadians(10); 
}

static void CorrectWeaponRotation(ABzWeapon* Weapon, FRotator& OutRotation, const FQuat& ViewRot, const FHitResult& Hit, float DeltaTime)
{
	if (Weapon)
	{
		FRotator Wanted;
		if (Hit.bBlockingHit)
		{
			const FVector& LocalDir = ViewRot.UnrotateVector(Hit.ImpactPoint - Weapon->GetActorLocation()).GetSafeNormal();
			Wanted = LocalDir.ToOrientationRotator();

			static float MinCos = FMath::Cos(MaxWeaponAngle);

			if (LocalDir.X < MinCos)
			{
				Wanted *= MaxWeaponAngle / FMath::Acos(LocalDir.X);
			}
		}
		else
		{
			Wanted = FRotator::ZeroRotator;
		}

		OutRotation = FMath::RInterpConstantTo(OutRotation, Wanted, DeltaTime, WeaponRotCorrectionSpeed);
		Weapon->SetActorRelativeRotation(OutRotation);
	}
	else
	{
		OutRotation = FRotator::ZeroRotator;
	}
}

void ABzPlayerMech::UpdateAim(float DeltaTime)
{
	CurrentAimHit.Reset();

	check(AimPointParent);
	const FTransform& Transform = AimPointParent->GetComponentTransform();
	const FQuat& Rotation = Transform.GetRotation();
	const FVector Start = Transform.GetLocation();
	const FVector End = Start + Rotation.Vector() * AimPointRayDistance;
	const UBzCombatSettings* Settings = UBzCombatSettings::Get();
	check(Settings);

	GetWorld()->LineTraceSingleByChannel(CurrentAimHit, Start, End, Settings->ProjectileChannel, CachedQueryParams);

	CorrectWeaponRotation(PrimaryWeapon, PrimaryWeaponCorrection, Rotation, CurrentAimHit, DeltaTime);
	CorrectWeaponRotation(SecondaryWeapon, SecondaryWeaponCorrection, Rotation, CurrentAimHit, DeltaTime);
}

void ABzPlayerMech::UpdateAbilityTargeting()
{
	if (!TargetingVisual)
	{
		return;
	}

	bool bTargetVisualHidden = true;
	if (AbilityComponent && AbilityComponent->IsTargetingAbility())
	{
		if (CurrentAimHit.bBlockingHit)
		{
			bTargetVisualHidden = false;
			TargetingVisual->SetActorLocation(CurrentAimHit.ImpactPoint);
		}
	}
	TargetingVisual->SetActorHiddenInGame(bTargetVisualHidden);
}

void ABzPlayerMech::TryJump()
{
	check(AbilityComponent);
	AbilityComponent->TryExecute(JumpAbilityHandle);
}

void ABzPlayerMech::TrySpeedBoost()
{
	check(AbilityComponent);
	AbilityComponent->TryExecute(SpeedAbilityHandle);
}

void ABzPlayerMech::SetSpeedBoostDuration(float Duration)
{
	SpeedBoostCooldown = Duration;
}

static TAutoConsoleVariable<float> CVarBzHeadbobPhaseDuration(TEXT("Bz.Headbob.PhaseDuration"), 1.5f, TEXT("Duration of a single stride cycle"), ECVF_Cheat);
static TAutoConsoleVariable<float> CVarBzHeadbobOffset(TEXT("Bz.Headbob.Offset"), 60.f, TEXT("How low the head can dip"), ECVF_Cheat);

void ABzPlayerMech::UpdateHeadBob(float DeltaTime)
{
	const float WantedCoef = (!IsSpeedBoostActive() && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
		? (GetVelocity().Size()/NormalSpeed)
		: 0.f;

	HeadBobCoef = FMath::FInterpConstantTo(HeadBobCoef, WantedCoef, DeltaTime, 2.f);

	HeadBobPhase = FMath::Modulo(HeadBobPhase + DeltaTime / CVarBzHeadbobPhaseDuration.GetValueOnGameThread(), 1.f);
	
	const float Angle = HeadBobPhase * TWO_PI;
	const float Raw = FMath::Sin(Angle) + FMath::Cos(2 * Angle);

	constexpr float InvRawRange = 1.f / (3.125f);
	constexpr float Correction = InvRawRange * 2 - 1;

	const float Normalized = InvRawRange * Raw + Correction; // Normalized sin(x) + cos(2x)

	AimPointParent->SetRelativeLocation(FVector(0,0, HeadBobCoef * Normalized * CVarBzHeadbobOffset.GetValueOnGameThread()));
	AimPoint->SetWorldTransform(UBzMathLibrary::UpdateSpringPoint(AimPointSpringState, AimPointParent->GetComponentTransform(), FBzSpringPointUpdateParams(), DeltaTime));
}

ABzWeapon* ABzPlayerMech::SpawnWeapon(const TSubclassOf<ABzWeapon>& WeaponClass, float Offset)
{
	ABzWeapon* NewWeapon = WeaponComponent->SpawnWeapon(WeaponClass, [&](ABzWeapon* Weapon)
		{
			Weapon->AttachToComponent(AimPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Weapon->SetActorRelativeLocation(FVector::RightVector * Offset);
			Weapon->OnFired.BindUObject(this, &ABzPlayerMech::BP_OnWeaponFired);
		});

	return NewWeapon;
}

void ABzPlayerMech::TryActivateAbility(const FBzAbilityHandle& Handle)
{
	check(AbilityComponent);
	if (AbilityComponent->GetTargetedAbility() == Handle)
		AbilityComponent->StopTargeting();
	else
		AbilityComponent->TryExecute(Handle, CurrentAimHit);
}
