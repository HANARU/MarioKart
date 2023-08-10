// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "KartPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API AKartPlayer : public ANinjaCharacter
{
	GENERATED_BODY()
	
public:
	AKartPlayer(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
		float Testfloat;
};
