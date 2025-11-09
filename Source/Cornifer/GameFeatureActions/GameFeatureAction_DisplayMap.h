// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_DisplayMap.generated.h"

class UDataAsset_Map;
/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, meta=(DisplayName="Cornifer: Load and Display Map"))
class CORNIFER_API UGameFeatureAction_DisplayMap : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Cornifer")
	UDataAsset_Map* MapData;

	UPROPERTY(EditAnywhere, Category = "Cornifer")
	bool bShowMapImmediately = true;

protected:
	virtual void AddToWorld(const FWorldContext& WorldContext) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

private:
	void ActivateForWorld(const UWorld* World);
	static void DeactivateForWorld(const UWorld* World);

	TSet<TWeakObjectPtr<const UWorld>> ActiveWorlds;
};
