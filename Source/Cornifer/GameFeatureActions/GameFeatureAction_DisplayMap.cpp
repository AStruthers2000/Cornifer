// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "GameFeatureAction_DisplayMap.h"
#include "CorniferSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"

void UGameFeatureAction_DisplayMap::AddToWorld(const FWorldContext& WorldContext)
{
	const UWorld* World = WorldContext.World();
	if (!World) return;

	if (World->GetNetMode() == NM_DedicatedServer) return;

	ActivateForWorld(World);
}

void UGameFeatureAction_DisplayMap::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	for (TWeakObjectPtr W : ActiveWorlds)
	{
		if (const UWorld* World = W.Get())
		{
			DeactivateForWorld(World);
		}
	}
	ActiveWorlds.Empty();

	Super::OnGameFeatureDeactivating(Context);
}

void UGameFeatureAction_DisplayMap::ActivateForWorld(const UWorld* World)
{
	if (!World || !MapData)
	{
		return;
	}

	if (IsValid(World))
	{
		ActiveWorlds.Add(World);
		if (const UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (auto* Subsystem = GameInstance->GetSubsystem<UCorniferSubsystem>())
			{
				Subsystem->InitializeMapSubsystem(MapData, bShowMapImmediately);
			}
		}
	}
}

void UGameFeatureAction_DisplayMap::DeactivateForWorld(const UWorld* World)
{
	if (!World) return;

	if (const UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (auto* Subsystem = GameInstance->GetSubsystem<UCorniferSubsystem>())
		{
			Subsystem->DeactivateMapSubsystem();
		}
	}
}


