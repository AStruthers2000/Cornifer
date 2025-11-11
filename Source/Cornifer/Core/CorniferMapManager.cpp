// Copyright (C) 2025 AStruthers2000 - All Rights Reserved


#include "CorniferMapManager.h"
#include "CorniferMapNode.h"
#include "CorniferSubsystem.h"
#include "CorniferUserWidget.h"
#include "DataAsset_Map.h"
#include "DataAsset_MapEdge.h"
#include "Blueprint/UserWidget.h"

void UCorniferMapManager::Initialize(UDataAsset_Map const* Map, const bool bImmediatelyShowMap)
{
	MapData = Map;
	bIsInitialized = true;
	
	// Construct all nodes
	for (auto const& NodePair : Map->MapNodes)
	{
		UCorniferMapNode* Node = NewObject<UCorniferMapNode>(this);
		Node->Initialize(NodePair.Value);

		// Add node to node map
		MapNodes.Add(NodePair.Key, Node);

		// Add node name to lookup bridge map
		NameToHitColor.Add(Node->GetNodeName(), NodePair.Key);
	}

	// Add neighbor relationships to all nodes
	for (auto const& NodeEdge : Map->MapEdges)
	{
		const FGameplayTag From = NodeEdge->FromNode;
		const FGameplayTag To = NodeEdge->ToNode;
		const EMapEdgeTraversalCost TraversalCost = NodeEdge->TraversalCost;
		
		UCorniferMapNode* FromNode = GetNodeFromNodeName(From);
		UCorniferMapNode* ToNode = GetNodeFromNodeName(To);

		FromNode->AddNodeNeighbor(To, TraversalCost);
		if (NodeEdge->bIsBidirectional)
		{
			ToNode->AddNodeNeighbor(From, TraversalCost);
		}
	}

	if (bImmediatelyShowMap)
	{
		ShowMap();
	}
}

void UCorniferMapManager::Deactivate()
{
	HideMap();
	MapData = nullptr;
	MapNodes.Empty();
	NameToHitColor.Empty();
	bIsInitialized = false;
}

void UCorniferMapManager::ShowMap()
{
	if (!bIsInitialized || !MapData) return;

	if (MapData->MapTexture.IsNull()) return;
	
	if (!MapWidget)
	{
		MapWidget = CreateWidget<UCorniferUserWidget>(GetWorld(), UCorniferUserWidget::StaticClass());
		if (!MapWidget) return;
		// Apply view configuration from data asset
		MapWidget->ConfigureMapView(MapData->MaxZoom, MapData->ZoomSpeed, MapData->InitialZoom);
		MapWidget->AddToViewport(MapData->ZOrder);
	}

	MapWidget->SetMapTexture(MapData->MapTexture);
}

void UCorniferMapManager::HideMap()
{
	if (!bIsInitialized || !MapWidget) return;

	MapWidget->RemoveFromParent();
	MapWidget->SetMapTexture(nullptr);
	MapWidget = nullptr;
}

UCorniferMapNode* UCorniferMapManager::GetNodeFromNodeName(const FGameplayTag NodeName) const
{
	if (!NameToHitColor.Contains(NodeName)) return nullptr;

	// Get the hit color associated with this name, which is the key to MapNodes
	EMapHitColor const NodeColor = NameToHitColor[NodeName];

	if (!MapNodes.Contains(NodeColor)) return nullptr;

	return MapNodes[NodeColor];
}
