// Fill out your copyright notice in the Description page of Project Settings.


#include "State_KartPlayer.h"
#include "Net/UnrealNetwork.h"

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

void AState_KartPlayer::Add1stItem(int Value)
{
	Current1stItem = Value;
	On1stItem.Broadcast(Current1stItem);

	/*switch (Value)
	{
	case 0:
	{
		Current1stItem = Value;
		break;
	}
	case 1:
	{
		Current1stItem = Value;
		break;
	}
	case 2:
	{
		Current1stItem = Value;
		break;
	}
	case 3:
	{
		Current1stItem = Value;
		break;
	}
	default:
		break;
	}*/
}

//void AState_KartPlayer::Add2ndItem(int Value)
//{
//}

void AState_KartPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, LapValue, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, CheckValue, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AState_KartPlayer, Item1st, COND_OwnerOnly);
}
