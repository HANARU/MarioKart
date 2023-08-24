// Fill out your copyright notice in the Description page of Project Settings.


#include "State_KartPlayer.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"
#include "KartInstance.h"

void AState_KartPlayer::OnRep_Lap(int OldLap)
{
	OnLapChanged.Broadcast(LapValue);
}

void AState_KartPlayer::OnRep_Check(int OldCheck)
{
	OnCheckChanged.Broadcast(CheckValue);
}

void AState_KartPlayer::OnRep_Item1st(int OldValue)
{
	On1stItem.Broadcast(Item1st);
}

void AState_KartPlayer::OnRep_Item2nd(int OldValue)
{
	On2ndItem.Broadcast(Item2nd);
}

void AState_KartPlayer::AddLapData(int Value)
{
	switch (Value)
	{
	case 8 :
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("CheckPoint Overlapped"));
		CurrentCheckPoint += 1;
		OnCheckChanged.Broadcast(CurrentCheckPoint);
		break;
	}
	case 9 :
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("LapPoint Overlapped"));
		if (CurrentCheckPoint > 2)
		{
			CurrentLapPoint += 1;
			CurrentCheckPoint = 0;
			OnCheckChanged.Broadcast(CurrentCheckPoint);
			OnLapChanged.Broadcast(CurrentLapPoint);
			break;
		}
		else
		{
			break;
		}
	}

	default:
		break;
	}
}

void AState_KartPlayer::AddItem()
{
	if (Current1stItem == 12 && Current2ndItem == 12)
	{
		Current1stItem = SelectRandomItem();
		On1stItem.Broadcast(Current1stItem);
	}
	else if(Current1stItem != 12 && Current2ndItem == 12)
	{
		Current2ndItem = Current1stItem;
		Current1stItem = SelectRandomItem();
		On1stItem.Broadcast(Current1stItem);
		On2ndItem.Broadcast(Current2ndItem);
	}
		
	UE_LOG(LogTemp, Warning, TEXT("1st Item : %d, 2nd Item : %d"), Current1stItem, Current2ndItem);
	
}

int AState_KartPlayer::SelectRandomItem()
{
	return FMath::RandRange(0, 3);
}

void AState_KartPlayer::UseItem()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Used Item"));
}

void AState_KartPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, LapValue, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, CheckValue, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, Item1st, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, Item2nd, COND_OwnerOnly);
}

void AState_KartPlayer::BeginPlay()
{
    Super::BeginPlay();

	gi = Cast<UKartInstance>(GetGameInstance());

	if (gi != nullptr /*&& GetPawn()->GetController() != nullptr && GetPawn()->GetController()->IsLocalPlayerController()*/)
	{
		SetMyName(gi->MySessionName);
	}
}

void AState_KartPlayer::SetMyName_Implementation(const FString& myName)
{
    SetPlayerName(myName);
}


