// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferSubsystem.h"
#include "CorniferUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"

void UCorniferSubsystem::ShowMap(UTexture2D* MapTexture, const int32 ZOrder)
{
	if (!MapTexture) return;

	if (!ImageWidget)
	{
		ImageWidget = CreateWidget<UCorniferUserWidget>(GetWorld(), UCorniferUserWidget::StaticClass());
		if (!ImageWidget) return;
		ImageWidget->AddToViewport(ZOrder);
	}
	ImageWidget->SetMapTexture(MapTexture);
}

void UCorniferSubsystem::HideMap()
{
	if (ImageWidget)
	{
		ImageWidget->RemoveFromParent();
		ImageWidget = nullptr;
	}
}
