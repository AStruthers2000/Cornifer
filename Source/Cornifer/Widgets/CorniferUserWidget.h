// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "Blueprint/UserWidget.h"
#include "Cornifer/CorniferDefaultValues.h"
#include "Engine/StreamableManager.h"
#include "CorniferUserWidget.generated.h"

class SCorniferZoomPan;
class UTexture2D;

UCLASS(BlueprintType)
class CORNIFER_API UCorniferUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void SetMapTexture(TSoftObjectPtr<UTexture2D> MapTexture);

	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void ResetView() const;

	// Configure map view behavior; call before or after widget is constructed.
	UFUNCTION(BlueprintCallable, Category = "Cornifer|Config")
	void ConfigureMapView(float InMaxZoom, float InZoomSpeed, float InInitialZoom);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	FStreamableManager StreamableManager;

	TSharedPtr<SCorniferZoomPan> SlateWidget;

	UPROPERTY()
	UTexture2D* CurrentMapTexture = nullptr;
	
	// Stored until Slate widget is built; applied immediately if already available
	float MaxZoom = GDefault_Max_Zoom_Level;
	float ZoomSpeed = GDefault_Zoom_Speed;
	float InitialZoom = GDefault_Initial_Zoom_Level;
};
