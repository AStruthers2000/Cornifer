// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferUserWidget.h"
#include "CoreMinimal.h"
#include "SCorniferZoomPan.h"
#include "Cornifer/CorniferDefaultValues.h"

TSharedRef<SWidget> UCorniferUserWidget::RebuildWidget()
{
	SlateWidget = SNew(SCorniferZoomPan)
		.Texture(CurrentMapTexture)
		.InitialZoom(InitialZoom)
		.MaxZoom(MaxZoom)
		.ZoomSpeed(ZoomSpeed);
	return SlateWidget.ToSharedRef();
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
