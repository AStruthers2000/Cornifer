// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cornifer/Types/MapEdgeTraversalCost.h"
#include "CorniferMapNode.generated.h"

class UDataAsset_MapNodeInformation;
class UDataAsset_MapNode;
/**
 * 
 */
UCLASS()
class CORNIFER_API UCorniferMapNode : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(const UDataAsset_MapNode* MapNode);
	void AddNodeNeighbor(FGameplayTag NeighborName, EMapEdgeTraversalCost TraversalCost);
	FGameplayTag GetNodeName() const { return NodeNameTag; }

private:
	UPROPERTY()
	FVector2D LocationUV;

	UPROPERTY()
	FGameplayTag NodeNameTag;

	UPROPERTY()
	UDataAsset_MapNodeInformation* MapNodeInformation;

	UPROPERTY()
	TMap<FGameplayTag, EMapEdgeTraversalCost> NodeNeighbors;
};
