// Copyright Epic Games, Inc. All Rights Reserved.

#include "CorniferRuntimeModule.h"

#define LOCTEXT_NAMESPACE "FCorniferRuntimeModule"

void FCorniferRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module
}

void FCorniferRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCorniferRuntimeModule, CorniferRuntime)
