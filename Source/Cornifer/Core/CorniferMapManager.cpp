// Copyright (C) 2025 AStruthers2000 - All Rights Reserved


#include "CorniferMapManager.h"
#include "CorniferMapNode.h"
#include "CorniferSubsystem.h"
#include "CorniferUserWidget.h"
#include "DataAsset_Map.h"
#include "DataAsset_MapEdge.h"
#include "Blueprint/UserWidget.h"
#include "Cornifer/Widgets/CorniferMapNodeWidget.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"

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

	// Ensure the main map widget exists
	if (!MapWidget)
	{
		MapWidget = CreateWidget<UCorniferUserWidget>(GetWorld(), UCorniferUserWidget::StaticClass());
		if (!MapWidget) return;
		// Apply view configuration from data asset
		MapWidget->ConfigureMapView(MapData->MaxZoom, MapData->ZoomSpeed, MapData->InitialZoom);
		MapWidget->AddToViewport(MapData->ZOrder);
	}

	// We need the texture size and any node widget classes; load them together
	TArray<FSoftObjectPath> ToLoad;
	ToLoad.Add(MapData->MapTexture.ToSoftObjectPath());
	for (const auto& Pair : MapData->MapNodes)
	{
		if (const UDataAsset_MapNode* NodeDA = Pair.Value)
		{
			if (!NodeDA->MapNodeWidgetClass.IsNull())
			{
				ToLoad.Add(NodeDA->MapNodeWidgetClass.ToSoftObjectPath());
			}
		}
	}

	static FStreamableManager Streamable;
	Streamable.RequestAsyncLoad(ToLoad, FStreamableDelegate::CreateWeakLambda(this, [this]()
	{
		// Texture now loaded; set it on the map widget (idempotent)
		MapWidget->SetMapTexture(MapData->MapTexture);

		UTexture2D* Texture = MapData->MapTexture.Get();
		if (!Texture) { return; }

		const float MapW = static_cast<float>(Texture->GetSizeX());
		const float MapH = static_cast<float>(Texture->GetSizeY());

		// Rebuild node widgets each time map is shown to match current data
		MapWidget->ClearMapWidgets();

		UWorld* World = GetWorld();
		if (!World) { return; }

		for (const auto& Pair : MapData->MapNodes)
		{
			UDataAsset_MapNode* NodeDA = Pair.Value;
			if (!NodeDA) { continue; }

			UClass* WidgetClass = nullptr;
			if (!NodeDA->MapNodeWidgetClass.IsNull())
			{
				WidgetClass = NodeDA->MapNodeWidgetClass.Get();
			}
			if (WidgetClass == nullptr)
			{
				WidgetClass = UCorniferMapNodeWidget::StaticClass();
			}

			UCorniferMapNodeWidget* NodeWidget = CreateWidget<UCorniferMapNodeWidget>(World, WidgetClass);
			if (!NodeWidget) { continue; }

			// Give the widget its data
			NodeWidget->InitializeFromNode(NodeDA);

			// Convert UV (0..1) to map pixels
			const FVector2D MapPx(NodeDA->LocationUV.X * MapW, NodeDA->LocationUV.Y * MapH);

			// Center alignment by default; no extra offset
			MapWidget->AddMapWidget(NodeWidget, MapPx, FVector2D(0.5f, 0.5f), FVector2D::ZeroVector);
		}
	}));
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
