// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_MapNodeInformation.generated.h"

/**
 * 
 */
UCLASS()
class CORNIFER_API UDataAsset_MapNodeInformation : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Cornifer|Map Nodes")
	FString NodeFriendlyName;
};
