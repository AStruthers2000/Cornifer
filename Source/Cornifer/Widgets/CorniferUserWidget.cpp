// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferUserWidget.h"
#include "CoreMinimal.h"
#include "SCorniferZoomPan.h"

TSharedRef<SWidget> UCorniferUserWidget::RebuildWidget()
{
	SlateWidget = SNew(SCorniferZoomPan)
		.Texture(CurrentMapTexture)
		.InitialZoom(InitialZoom)
		.MaxZoom(MaxZoom)
		.ZoomSpeed(ZoomSpeed);
	return SlateWidget.ToSharedRef();
}

void UCorniferUserWidget::SetMapTexture(UTexture2D* MapTexture)
{
	CurrentMapTexture = MapTexture;
	if (SlateWidget.IsValid())
	{
		SlateWidget->SetTexture(CurrentMapTexture);
	}
}

void UCorniferUserWidget::ResetView()
{
	if (SlateWidget.IsValid())
	{
		SlateWidget->ResetView();
	}
}

void UCorniferUserWidget::ConfigureMapView(const float InMaxZoom, const float InZoomSpeed)
{
	MaxZoom = FMath::Max(1.0f, InMaxZoom);
	ZoomSpeed = FMath::Max(1.01f, InZoomSpeed);

	if (SlateWidget.IsValid())
	{
		SlateWidget->SetMaxZoom(MaxZoom);
		SlateWidget->SetZoomSpeed(ZoomSpeed);
	}
}

void UCorniferUserWidget::ConfigureInitialZoom(const float InInitialZoom)
{
	InitialZoom = FMath::Max(0.1f, InInitialZoom);
	if (SlateWidget.IsValid())
	{
		SlateWidget->SetZoom(InitialZoom); // first paint after this will clamp to cover viewport
	}
}


