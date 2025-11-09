// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cornifer/Types/MapHitColor.h"
#include "Engine/StreamableManager.h"
#include "CorniferMapManager.generated.h"

class UCorniferUserWidget;
class UCorniferMapNode;
class UDataAsset_Map;
/**
 * 
 */
UCLASS()
class CORNIFER_API UCorniferMapManager : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(UDataAsset_Map const* Map, bool bImmediatelyShowMap);
	void Deactivate();

	void ShowMap();
	void HideMap();

	UCorniferMapNode* GetNodeFromNodeName(FGameplayTag NodeName) const;

private:
	UPROPERTY()
	bool bIsInitialized = false;
	
	UPROPERTY()
	UDataAsset_Map const* MapData;

	UPROPERTY()
	UCorniferUserWidget* MapWidget = nullptr;
	
	FStreamableManager StreamableManager;

	UPROPERTY()
	TMap<EMapHitColor, UCorniferMapNode*> MapNodes;

	UPROPERTY()
	TMap<FGameplayTag, EMapHitColor> NameToHitColor;
};
