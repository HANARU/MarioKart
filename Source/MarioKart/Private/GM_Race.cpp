#include "GM_Race.h"
#include "KartPlayer.h"
#include "ItemBase.h"
#include "Kismet/KismetStringLibrary.h"

void AGM_Race::BeginPlay()
{
	Super::BeginPlay();
}

void AGM_Race::ItemOverlaped(AKartPlayer* Player)
{
	FString KartName = UKismetStringLibrary::Conv_ObjectToString(Player);
	FString ItemNum = UKismetStringLibrary::Conv_IntToString(RandomItem());
	this->ItemSignature.ExecuteIfBound(RandomItem());
}

int32 AGM_Race::RandomItem()
{
	return FMath::RandRange(0, 1);
}
