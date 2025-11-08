// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "GameFeatureAction_DisplayMap.h"
#include "CorniferSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"

void UGameFeatureAction_DisplayMap::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	if (!World) return;

	if (World->GetNetMode() == NM_DedicatedServer) return;

	ActivateForWorld(World);
}

void UGameFeatureAction_DisplayMap::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("[UGameFeatureAction_DisplayMap::OnGameFeatureDeactivating]: Deactivating"));

	for (TWeakObjectPtr W : ActiveWorlds)
	{
		if (UWorld* World = W.Get())
		{
			DeactivateForWorld(World);
		}
	}
	ActiveWorlds.Empty();

	Super::OnGameFeatureDeactivating(Context);
}

void UGameFeatureAction_DisplayMap::ActivateForWorld(UWorld* World)
{
	if (!World || MapTexture.IsNull())
	{
		return;
	}

	StreamableManager.RequestAsyncLoad(MapTexture.ToSoftObjectPath(), [this, World]()
	{
		if (IsValid(World))
		{
			if (UTexture2D* Texture = MapTexture.Get())
			{
				if (const UGameInstance* GameInstance = World->GetGameInstance())
				{
					if (auto* Subsystem = GameInstance->GetSubsystem<UCorniferSubsystem>())
					{
						Subsystem->ShowMap(Texture, ZOrder);
					}
				}
			}
		}
	});
}

void UGameFeatureAction_DisplayMap::DeactivateForWorld(const UWorld* World)
{
	if (!World) return;

	if (const UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (auto* Subsystem = GameInstance->GetSubsystem<UCorniferSubsystem>())
		{
			Subsystem->HideMap();
		}
	}
}


