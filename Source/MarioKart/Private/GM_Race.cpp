// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Race.h"

void AGM_Race::BeginPlay()
{
	Super::BeginPlay();
}

void AGM_Race::Overlap_Lap_System(ANinjaCharacter* Player, bool bIsCheckPoint, bool bIsLap)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Check"));

}
