// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferUserWidget.h"
#include "CoreMinimal.h"
#include "SCorniferMapView.h"
#include "Cornifer/CorniferDefaultValues.h"
// For programmatic focus
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/WidgetTree.h"

TSharedRef<SWidget> UCorniferUserWidget::RebuildWidget()
{
	SlateWidget = SNew(SCorniferMapView)
		.Texture(CurrentMapTexture)
		.InitialZoom(InitialZoom)
		.MaxZoom(MaxZoom)
		.ZoomSpeed(ZoomSpeed);
	return SlateWidget.ToSharedRef();
}

void UCorniferUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Make the UMG widget focusable so focus can reside within it.
	SetIsFocusable(true);

	// Ensure our Slate child is focusable and receives focus immediately so the first click can pan.
	if (SlateWidget.IsValid())
	{
		FSlateApplication::Get().SetKeyboardFocus(SlateWidget, EFocusCause::SetDirectly);
	}
}

void UCorniferUserWidget::SetMapTexture(TSoftObjectPtr<UTexture2D> MapTexture)
{
	StreamableManager.RequestAsyncLoad(MapTexture.ToSoftObjectPath(), [this, MapTexture]()
	{
		CurrentMapTexture = MapTexture.Get();
		if (SlateWidget.IsValid())
		{
			SlateWidget->SetTexture(CurrentMapTexture);
		}
	});
}

void UCorniferUserWidget::ResetView() const
{
	if (SlateWidget.IsValid())
	{
		SlateWidget->ResetView();
	}
}

void UCorniferUserWidget::ConfigureMapView(const float InMaxZoom, const float InZoomSpeed, const float InInitialZoom)
{
	InitialZoom = FMath::Clamp(InInitialZoom, GMinimum_Initial_Zoom_Level, InMaxZoom);
	MaxZoom = FMath::Max(GMinimum_Max_Zoom_Level, InMaxZoom);
	ZoomSpeed = FMath::Max(GMinimum_Zoom_Speed, InZoomSpeed);
	
	if (SlateWidget.IsValid())
	{
		SlateWidget->SetMaxZoom(InMaxZoom);
		SlateWidget->SetZoomSpeed(InZoomSpeed);
		SlateWidget->SetZoom(InInitialZoom); // first paint after this will clamp to cover viewport
		UE_LOG(LogTemp, Display, TEXT("Map View Configured"));
	}
}

void UCorniferUserWidget::AddMapWidget(UWidget* Child, FVector2D MapPosition, FVector2D Alignment, FVector2D Offset)
{
	if (!Child || !SlateWidget.IsValid())
	{
		return;
	}

	// Take ownership of the widget in the UMG widget tree
	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this);
	}

	// Add widget to the tree (not as root, but as a child we're managing)
	Child->Rename(nullptr, WidgetTree);

	// Add the widget's Slate representation to the map view
	SlateWidget->AddSlot()
		.MapPosition(MapPosition)
		.Alignment(Alignment)
		.PixelOffset(Offset)
		[
			Child->TakeWidget()
		];
}

void UCorniferUserWidget::ClearMapWidgets()
{
	if (SlateWidget.IsValid())
	{
		SlateWidget->ClearChildren();
	}
}

FVector2D UCorniferUserWidget::MapToScreen(FVector2D MapPx) const
{
	if (!SlateWidget.IsValid())
	{
		return FVector2D::ZeroVector;
	}

	// Get the geometry of the Slate widget
	FGeometry Geometry = GetCachedGeometry();
	return SlateWidget->MapToScreen(MapPx, Geometry);
}

FVector2D UCorniferUserWidget::ScreenToMap(FVector2D ScreenPx) const
{
	if (!SlateWidget.IsValid())
	{
		return FVector2D::ZeroVector;
	}

	// Get the geometry of the Slate widget
	FGeometry Geometry = GetCachedGeometry();
	return SlateWidget->ScreenToMap(ScreenPx, Geometry);
}

void UCorniferUserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	SlateWidget.Reset();
}
