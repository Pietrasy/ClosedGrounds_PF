// Pietras

using UnrealBuildTool;
using System.Collections.Generic;

public class ClosedGroundsTarget : TargetRules
{
	public ClosedGroundsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "ClosedGrounds" } );
	}
}
