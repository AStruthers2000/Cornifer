// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "CorniferMapManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CorniferSubsystem.generated.h"

class UCorniferUserWidget;
class UTexture2D;

UCLASS()
class CORNIFER_API UCorniferSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void InitializeMapSubsystem(UDataAsset_Map const* MapData, bool bImmediatelyShowMap = true);
	void DeactivateMapSubsystem();

	UFUNCTION(BlueprintPure, Category = "Cornifer")
	UCorniferMapManager* GetMapManager() const { return MapManager; }
	
	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void ShowMap() const;

	UFUNCTION(BlueprintCallable, Category = "Cornifer")
	void HideMap() const;

private:
	bool bIsSubsystemInitialized = false;

	UPROPERTY(Transient)
	UCorniferMapManager* MapManager = nullptr;
};
