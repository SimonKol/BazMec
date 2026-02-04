// (c) Simon Kolciter


#include "BzPickup.h"

#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBzPickup, Log, All);

ABzPickup::ABzPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InitialLifeSpan = 180.f;

	USphereComponent* Root = CreateDefaultSubobject<USphereComponent>("Root");
	SetRootComponent(Root);
	Root->SetCollisionProfileName("Bz_Pickup");
	Root->SetSphereRadius(200.f);

	Visual = CreateDefaultSubobject<USceneComponent>("Visual");
	Visual->SetupAttachment(Root);
	Visual->SetRelativeLocation(FVector::UpVector * 50.f);
}

void ABzPickup::CheckPickup(AActor* User)
{
	check(User);

	if (User->IsActorBeingDestroyed())
	{
		return;
	}

	UE_LOG(LogBzPickup, Verbose, TEXT("Checking if (%s) can pick up (%s)"), *User->GetName(), *GetName());
	if (CanPickup(User))
	{
		UE_LOG(LogBzPickup, Display, TEXT("User (%s) picks up (%s) @ %s"), *User->GetName(), *GetName(), *GetActorLocation().ToCompactString());
		DoPickup(User);
		Destroy();
	}
}

void ABzPickup::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABzPickup::OnActorOverlap);
	
	URotatingMovementComponent* Rotor = NewObject<URotatingMovementComponent>(this);
	Rotor->RegisterComponent();
	Rotor->RotationRate = FRotator(0, 240, 0);
	Rotor->SetUpdatedComponent(Visual);
}
