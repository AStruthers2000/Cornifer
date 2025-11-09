// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_MapNode.generated.h"

class UDataAsset_MapNodeInformation;
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
};
