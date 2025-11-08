// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferUserWidget.h"
#include "SCorniferZoomPan.h"

TSharedRef<SWidget> UCorniferUserWidget::RebuildWidget()
{
	SlateWidget = SNew(SCorniferZoomPan).Texture(CurrentMapTexture).InitialZoom(1.f);
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


