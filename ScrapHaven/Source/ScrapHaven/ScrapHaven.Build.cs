// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ScrapHaven : ModuleRules
{
	public ScrapHaven(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ScrapHaven",
			"ScrapHaven/Variant_Platforming",
			"ScrapHaven/Variant_Platforming/Animation",
			"ScrapHaven/Variant_Combat",
			"ScrapHaven/Variant_Combat/AI",
			"ScrapHaven/Variant_Combat/Animation",
			"ScrapHaven/Variant_Combat/Gameplay",
			"ScrapHaven/Variant_Combat/Interfaces",
			"ScrapHaven/Variant_Combat/UI",
			"ScrapHaven/Variant_SideScrolling",
			"ScrapHaven/Variant_SideScrolling/AI",
			"ScrapHaven/Variant_SideScrolling/Gameplay",
			"ScrapHaven/Variant_SideScrolling/Interfaces",
			"ScrapHaven/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
