// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "Subsystems/GameInstanceSubsystem.h"
#include "CorniferSubsystem.generated.h"

class UCorniferUserWidget;
class UTexture2D;

UCLASS()
class CORNIFER_API UCorniferSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void ShowMap(UTexture2D* MapTexture, int32 ZOrder = 100);

	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void HideMap();

private:
	UPROPERTY()
	UCorniferUserWidget* ImageWidget = nullptr;
};
