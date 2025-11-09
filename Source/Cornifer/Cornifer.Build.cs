// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Cornifer : ModuleRules
{
	public Cornifer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange([
			Path.Combine(PluginDirectory, "Source/Cornifer/Core")
		]);
		
		PrivateIncludePaths.AddRange([
			Path.Combine(PluginDirectory, "Source/Cornifer/DataAssets"), 
			Path.Combine(PluginDirectory, "Source/Cornifer/GameFeatureActions"),
			Path.Combine(PluginDirectory, "Source/Cornifer/Widgets")
		]);

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG",
			"InputCore",
			"GameFeatures",
			"GameplayTags",
			"ModularGameplay"
		]);

		PrivateDependencyModuleNames.AddRange([]);

		DynamicallyLoadedModuleNames.AddRange([]);
	}
}
