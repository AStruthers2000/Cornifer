// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cornifer/Types/MapEdgeTraversalCost.h"
#include "Engine/DataAsset.h"
#include "DataAsset_MapEdge.generated.h"

/**
 * 
 */
UCLASS()
class CORNIFER_API UDataAsset_MapEdge : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	FGameplayTag FromNode;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	FGameplayTag ToNode;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	bool bIsBidirectional;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	EMapEdgeTraversalCost TraversalCost;
};
