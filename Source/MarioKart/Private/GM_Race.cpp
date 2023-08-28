#include "GM_Race.h"
#include "KartPlayer.h"
#include "ItemBase.h"
#include "State_KartPlayer.h"
#include "Kismet/KismetStringLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "ItemInfo.h"
#include "Kismet/GameplayStatics.h"


AGM_Race::AGM_Race()
{
	PlayerStateClass = AState_KartPlayer::StaticClass();

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/7_System/DataList/DT_ItemInfo.DT_ItemInfo'"));
	if (DataTable.Succeeded())
	{
		ItemDataTable = DataTable.Object;
	}
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
	int PlayerNums = GetNumPlayers();
	if (PlayerNums == 2)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("True"));
		Able2Play = true;
		StartCountDown();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("False"));
	}
}

void AGM_Race::StartCountDown_Implementation()
{
}

void AGM_Race::ItemOverlaped(AKartPlayer* Player)
{
	FString KartName = UKismetStringLibrary::Conv_ObjectToString(Player);
	FString ItemNum = UKismetStringLibrary::Conv_IntToString(RandomItem());
	this->ItemSignature.ExecuteIfBound(RandomItem());
}

void AGM_Race::ItemOverlaped(UItemComponent* ItemComp)
{
	FString ItemNum = UKismetStringLibrary::Conv_IntToString(RandomItem());
	//this->ItemSignature.ExecuteIfBound(RandomItem());
}

int32 AGM_Race::RandomItem()
{
	return FMath::RandRange(0, 9);
}
