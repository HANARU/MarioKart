// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "MarioKartHud.generated.h"


UCLASS(config = Game)
class AMarioKartHud : public AHUD
{
	GENERATED_BODY()

public:
	AMarioKartHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
