using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalShooterTarget : TargetRules
{
	public SurvivalShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "SurvivalShooter" } );
	}
}