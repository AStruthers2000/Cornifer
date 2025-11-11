// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once

#include "Blueprint/UserWidget.h"
#include "CorniferMapNodeWidget.generated.h"

class UDataAsset_MapNode;

/**
 * Base widget for a map node pin. Designers can subclass this in Blueprint and
 * assign per-node via DataAsset_MapNode::MapNodeWidgetClass.
 */
UCLASS(BlueprintType, Blueprintable)
class CORNIFER_API UCorniferMapNodeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Initialize this widget from the node's data. Implement in BP as needed. */
    UFUNCTION(BlueprintImplementableEvent, Category="Cornifer|MapNode")
    void InitializeFromNode(UDataAsset_MapNode* NodeData);
};
