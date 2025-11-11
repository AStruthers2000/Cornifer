// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "Blueprint/UserWidget.h"
#include "Cornifer/CorniferDefaultValues.h"
#include "Engine/StreamableManager.h"
#include "CorniferUserWidget.generated.h"

class SCorniferMapView;
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

	// Add a widget at a specific map position (in map pixel coordinates)
	UFUNCTION(BlueprintCallable, Category = "Cornifer|MapWidgets")
	void AddMapWidget(UWidget* Child, FVector2D MapPosition, FVector2D Alignment, FVector2D Offset);

	// Clear all map widgets
	UFUNCTION(BlueprintCallable, Category = "Cornifer|MapWidgets")
	void ClearMapWidgets();

	// Convert map-space coordinates to screen-space coordinates
	UFUNCTION(BlueprintCallable, Category = "Cornifer|Coordinates")
	FVector2D MapToScreen(FVector2D MapPx) const;

	// Convert screen-space coordinates to map-space coordinates
	UFUNCTION(BlueprintCallable, Category = "Cornifer|Coordinates")
	FVector2D ScreenToMap(FVector2D ScreenPx) const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	FStreamableManager StreamableManager;

	TSharedPtr<SCorniferMapView> SlateWidget;

	UPROPERTY()
	UTexture2D* CurrentMapTexture = nullptr;
	
	// Stored until Slate widget is built; applied immediately if already available
	float MaxZoom = GDefault_Max_Zoom_Level;
	float ZoomSpeed = GDefault_Zoom_Speed;
	float InitialZoom = GDefault_Initial_Zoom_Level;
};
