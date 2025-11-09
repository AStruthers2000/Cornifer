// Copyright (C) 2025 AStruthers2000 - All Rights Reserved


#include "CorniferMapNode.h"

#include "DataAsset_MapNode.h"

void UCorniferMapNode::Initialize(const UDataAsset_MapNode* MapNode)
{
	LocationUV = MapNode->LocationUV;
	NodeNameTag = MapNode->NodeNameTag;
	MapNodeInformation = MapNode->MapNodeInformation;
}

void UCorniferMapNode::AddNodeNeighbor(const FGameplayTag NeighborName, const EMapEdgeTraversalCost TraversalCost)
{
	if (!NodeNeighbors.Contains(NeighborName))
	{
		NodeNeighbors.Add(NeighborName, TraversalCost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCorniferMapNode::AddNodeNeighbor] Node %s already exists in node %s's neighbor map"), *NeighborName.ToString(), *NodeNameTag.ToString());
	}
}
