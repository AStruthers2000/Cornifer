#pragma once

UENUM(BlueprintType)
enum class EMapEdgeTraversalCost : uint8
{
	Default = 0,
	Euclidean,
	Checkerboard
};