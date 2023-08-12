// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NinjaCharacter.h"
#include "I_Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Interact : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MARIOKART_API II_Interact
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Overlap_Lap_System(ANinjaCharacter* Player, bool bIsCheckPoint, bool bIsLap);
};