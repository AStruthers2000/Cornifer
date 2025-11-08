// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "Engine/StreamableManager.h"
#include "GameFeatureAction_DisplayMap.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, meta=(DisplayName="Cornifer: Load and Display Map"))
class CORNIFER_API UGameFeatureAction_DisplayMap : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Cornfier")
	TSoftObjectPtr<UTexture2D> MapTexture;

	UPROPERTY(EditAnywhere, Category = "Cornifer")
	int32 ZOrder = 999;

protected:
	virtual void AddToWorld(const FWorldContext& WorldContext) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

private:
	void ActivateForWorld(UWorld* World);
	static void DeactivateForWorld(const UWorld* World);

	TSet<TWeakObjectPtr<UWorld>> ActiveWorlds;
	FStreamableManager StreamableManager;
};
