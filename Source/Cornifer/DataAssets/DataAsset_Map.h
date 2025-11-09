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
	
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TMap<EMapHitColor, UDataAsset_MapNode*> MapNodes;

	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Data")
	TArray<UDataAsset_MapEdge*> MapEdges;
};
