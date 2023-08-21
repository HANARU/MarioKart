#include "GM_Race.h"
#include "KartPlayer.h"
#include "ItemBase.h"
#include "Kismet/KismetStringLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "ItemInfo.h"


AGM_Race::AGM_Race()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/7_System/DataList/DT_ItemInfo.DT_ItemInfo'"));
	if (DataTable.Succeeded())
	{
		ItemDataTable = DataTable.Object;
	}
}

int32 AGM_Race::GetNumberOfPlayersInLevel()
{
	int32 PlayerCount = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && PlayerController->GetPawn())
		{
			PlayerCount++;
		}
	}
	return PlayerCount;
}


void AGM_Race::Tick(float Deltatime)
{
	CheckAble2Play();
}

void AGM_Race::BeginPlay()
{
	Super::BeginPlay();


	if (ItemDataTable != nullptr)
	{
		
		FItemInfo* ItemDataRow = ItemDataTable->FindRow<FItemInfo>(FName(*FString::FromInt(ItemName)), TEXT("Context String"));

		// 아이템 인덱스를 로그로 출력합니다.
		UE_LOG(LogTemp, Warning, TEXT("ItemIndex: %d"), ItemName);
		
	}
}

void AGM_Race::CheckAble2Play()
{
	if (Able2Play)
	{
		return;
	}
	else
	{
		if (GetNumberOfPlayersInLevel() == 2)
		{
			Able2Play = true;
		}
		else
		{
			Able2Play = false;
		}
	}
}

void AGM_Race::ItemOverlaped(AKartPlayer* Player)
{
	FString KartName = UKismetStringLibrary::Conv_ObjectToString(Player);
	FString ItemNum = UKismetStringLibrary::Conv_IntToString(RandomItem());
	this->ItemSignature.ExecuteIfBound(RandomItem());
}

void AGM_Race::ItemOverlaped(UItemComponent* ItemCompo)
{
	FString ItemNum = UKismetStringLibrary::Conv_IntToString(RandomItem());
	this->ItemSignature.ExecuteIfBound(RandomItem());
}

int32 AGM_Race::RandomItem()
{
	return FMath::RandRange(0, 1);
}
