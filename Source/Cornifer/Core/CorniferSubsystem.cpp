// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "CorniferSubsystem.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"

void UCorniferSubsystem::InitializeMapSubsystem(UDataAsset_Map const* MapData, const bool bImmediatelyShowMap)
{
	MapManager = NewObject<UCorniferMapManager>(this);
	MapManager->Initialize(MapData, bImmediatelyShowMap);

	bIsSubsystemInitialized = true;
}

void UCorniferSubsystem::DeactivateMapSubsystem()
{
	MapManager->Deactivate();
	MapManager = nullptr;

	bIsSubsystemInitialized = false;
}

void UCorniferSubsystem::ShowMap() const
{
	if (!bIsSubsystemInitialized || !MapManager) return;

	MapManager->ShowMap();
}

void UCorniferSubsystem::HideMap() const
{
	if (!bIsSubsystemInitialized || !MapManager) return;

	MapManager->HideMap();
}
