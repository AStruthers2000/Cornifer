// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "DataAsset_MapNode.generated.h"

class UDataAsset_MapNodeInformation;
class UCorniferMapNodeWidget;
/**
 * 
 */
UCLASS()
class CORNIFER_API UDataAsset_MapNode : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	FVector2D LocationUV;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	FGameplayTag NodeNameTag;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	UDataAsset_MapNodeInformation* MapNodeInformation;

	/** Optional widget class to visualize this node on the map. Can be overridden per-node. */
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes|UI", meta=(AllowedClasses="CorniferMapNodeWidget"))
	TSoftClassPtr<UCorniferMapNodeWidget> MapNodeWidgetClass;
};
