// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "Blueprint/UserWidget.h"
#include "CorniferUserWidget.generated.h"

class SCorniferZoomPan;
class UTexture2D;

UCLASS(BlueprintType)
class CORNIFER_API UCorniferUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void SetMapTexture(UTexture2D* MapTexture);

	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void ResetView();

	// Configure map view behavior; call before or after widget is constructed.
	UFUNCTION(BlueprintCallable, Category = "Cornifer|Config")
	void ConfigureMapView(float InMaxZoom, float InZoomSpeed);

	// Configure initial zoom used when the Slate widget is built
	UFUNCTION(BlueprintCallable, Category = "Cornifer|Config")
	void ConfigureInitialZoom(float InInitialZoom);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SCorniferZoomPan> SlateWidget;
	
	UPROPERTY()
	UTexture2D* CurrentMapTexture = nullptr;

	// Stored until Slate widget is built; applied immediately if already available
	float MaxZoom = 10.f;
	float ZoomSpeed = 1.1f;
	float InitialZoom = 1.f;
};
