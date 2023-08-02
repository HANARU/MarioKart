// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarioKartGameMode.h"
#include "MarioKartPawn.h"
#include "MarioKartHud.h"

AMarioKartGameMode::AMarioKartGameMode()
{
	DefaultPawnClass = AMarioKartPawn::StaticClass();
	HUDClass = AMarioKartHud::StaticClass();
}
