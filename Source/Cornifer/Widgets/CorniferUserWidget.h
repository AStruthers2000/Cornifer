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

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SCorniferZoomPan> SlateWidget;
	
	UPROPERTY()
	UTexture2D* CurrentMapTexture = nullptr;
};
