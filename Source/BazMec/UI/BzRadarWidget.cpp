// (c) Simon Kolciter


#include "BzRadarWidget.h"

#include "BzRadarConfigAsset.h"

#include "Radar/BzRadarSubsystem.h"
#include "Radar/BzRadarComponent.h"

#include "Widgets/SOverlay.h"
#include "Styling/UMGCoreStyle.h"

class SBzRadarItem : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SBzRadarItem) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const FSlateBrush* Brush);
	void SetIsInRange(bool Value);
private:
};

void SBzRadarItem::Construct(const FArguments& InArgs, const FSlateBrush* Brush)
{
	ChildSlot
		[
			SNew(SImage)
				.Image(Brush ? Brush : FUMGCoreStyle::Get().GetBrush("Icons.Cross"))
		];
}

void SBzRadarItem::SetIsInRange(bool Value)
{
	ChildSlot.GetWidget()->SetRenderOpacity(Value ? 1 : .1f);
}

class SBzRadarMap : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SBzRadarMap) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBzRadarSubsystem* Subsystem, const UBzRadarConfigAsset* Config);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetPawn(APawn* Actor);

private:

	void AddComponent(UBzRadarComponent* Component);
	void RemoveComponent(UBzRadarComponent* Component);

	struct FItem
	{
		explicit FItem(const TSharedPtr<SBzRadarItem>& InRadarItem) : RadarItem(InRadarItem) {}
		TSharedPtr<SBzRadarItem> RadarItem;
	};

	TWeakObjectPtr<UBzRadarSubsystem> RadarSubsystem;
	TWeakObjectPtr<APawn> OwningPawn;

	decltype(UBzRadarConfigAsset::Icons) Icons;
	float InvMapRange = 0;

	TSharedPtr<SOverlay> MapOverlay;
	TMap<const UBzRadarComponent*, FItem> Items;
};

void SBzRadarMap::Construct(const FArguments& InArgs, UBzRadarSubsystem* Subsystem, const UBzRadarConfigAsset* Config)
{
	RadarSubsystem = Subsystem;

	ChildSlot
		[
			SAssignNew(MapOverlay, SOverlay)
		];

	if (Config)
	{
		Icons = Config->Icons;
		InvMapRange = 1.f / Config->MinimumRange;
	}

	if (Subsystem)
	{
		Subsystem->OnItemRegistered.AddSP(this, &SBzRadarMap::AddComponent);
		Subsystem->OnItemUnregistered.AddSP(this, &SBzRadarMap::RemoveComponent);

		for (UBzRadarComponent* Comp : Subsystem->GetActiveComponents())
		{
			AddComponent(Comp);
		}
	}
}

void SBzRadarMap::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	SCOPED_NAMED_EVENT(BzRadarMap_Tick, FColor::Black);

	FMatrix2x2 RotMatrix;
	FVector MyLocation = FVector::ZeroVector;

	const FVector2D Extent = .5f * MapOverlay->GetTickSpaceGeometry().GetLocalSize() - FVector2D(16.f);

	if (APawn* Pawn = OwningPawn.Get())
	{
		MyLocation = Pawn->GetActorLocation();
		RotMatrix = FMatrix2x2(FQuat2f(FMath::DegreesToRadians(-Pawn->GetControlRotation().Yaw)));
	}

	const float Multiplier = FMath::Min(Extent.X, Extent.Y) * InvMapRange;

	for (const auto& Pair : Items)
	{
		bool bVisible = Pair.Key->IsEnabled();
		SBzRadarItem* RadarItem = Pair.Value.RadarItem.Get();
		check(RadarItem);

		if (bVisible)
		{
			const FVector WorldOffset = Pair.Key->GetOwner()->GetActorLocation() - MyLocation;
			FVector2D Mapped = RotMatrix.TransformVector(FVector2D(WorldOffset.Y, -WorldOffset.X) * Multiplier);
			const FVector2D Abs = Mapped.GetAbs();

			bool bInRange = true;

			if (Abs.X > Extent.X || Abs.Y > Extent.Y) // project to rectangular minimap
			{
				FVector2D Divisor = Abs / Extent;
				Mapped /= FMath::Max(Divisor.X, Divisor.Y);
				bInRange = false;
			}

			RadarItem->SetRenderTransform(Mapped);
			RadarItem->SetIsInRange(bInRange);
		}
		RadarItem->SetVisibility(bVisible ? EVisibility::Visible : EVisibility::Hidden);
	}
}

void SBzRadarMap::SetPawn(APawn* Pawn)
{
	OwningPawn = Pawn;
}

void SBzRadarMap::AddComponent(UBzRadarComponent* Component)
{
	TSharedPtr<SBzRadarItem> RadarItem;

	MapOverlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(RadarItem, SBzRadarItem, Icons.Find(Component->ObjectTag))
		];

	Items.Add(Component, FItem(RadarItem));
}

void SBzRadarMap::RemoveComponent(UBzRadarComponent* Component)
{
	if (FItem* Item = Items.Find(Component))
	{
		MapOverlay->RemoveSlot(Item->RadarItem.ToSharedRef());
		Items.Remove(Component);
	}
}

void UBzRadarWidget::SetPawn(APawn* Pawn)
{
	OwningPawn = Pawn;
	SynchronizeProperties();
}

void UBzRadarWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (SBzRadarMap* Widget = MyWidget.Get())
	{
		Widget->SetPawn(OwningPawn);
	}
}

void UBzRadarWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyWidget.Reset();
}

TSharedRef<SWidget> UBzRadarWidget::RebuildWidget()
{
	UBzRadarSubsystem* RadarSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UBzRadarSubsystem>() : nullptr;
	MyWidget = SNew(SBzRadarMap, RadarSubsystem, Config);
	return MyWidget.ToSharedRef();
}
