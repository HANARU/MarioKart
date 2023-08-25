// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MarioKart : ModuleRules
{
	public MarioKart(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemSteam", "PhysXVehicles", "UMG", "Niagara" });

		PublicDefinitions.Add("HMD_MODULE_INCLUDED=1");

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
