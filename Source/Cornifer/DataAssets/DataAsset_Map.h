// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Cornifer/Types/MapHitColor.h"
#include "Engine/DataAsset.h"
#include "DataAsset_Map.generated.h"

class UDataAsset_MapEdge;
class UDataAsset_MapNode;
/**
 * 
 */
UCLASS()
class CORNIFER_API UDataAsset_Map : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TSoftObjectPtr<UTexture2D> MapTexture;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TSoftObjectPtr<UTexture2D> MapCollisionData;
	
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	int32 ZOrder = 999;
	
	// Initial zoom level when the map first opens. 1.0 = original size
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map View", meta=(ClampMin="0.1", UIMin="0.1"))
	float InitialZoom = 1.f;

	// Maximum zoom level allowed for the map view (upper clamp). 1.0 = original size, >1.0 = zoom in
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map View", meta=(ClampMin="1.0", UIMin="1.0"))
	float MaxZoom = 10.f;

	// Multiplicative zoom speed factor per mouse wheel step (> 1.0 means zoom in/out faster)
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map View", meta=(ClampMin="1.01", UIMin="1.01"))
	float ZoomSpeed = 1.1f;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TMap<EMapHitColor, UDataAsset_MapNode*> MapNodes;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TArray<UDataAsset_MapEdge*> MapEdges;
};
