#include "LapVolume.h"
#include "GM_Race.h"
#include "KartPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Net/UnrealNetwork.h"

ALapVolume::ALapVolume()
{
	//PrimaryActorTick.bCanEverTick = true;
	CheckVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CheckVolume;
	CheckVolume->SetCollisionProfileName(FName(TEXT("ItemBox")));
	
	bReplicates = true;
}

void ALapVolume::BeginPlay()
{
	Super::BeginPlay();

	GMRace = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));

	CheckVolume->OnComponentBeginOverlap.AddDynamic(this, &ALapVolume::OnOverlapBegin);

}

// Called every frame
void ALapVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALapVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKartPlayer* Player = Cast<AKartPlayer>(OtherActor);
	if (Player != nullptr)
	{
		SetOwner(Player);
		Confirm_Goal(Player);
		Confirm_CheckPoint(Player);
	}
}

void ALapVolume::Confirm_Goal(AKartPlayer* Player)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Goal"));

	if (bIsThisLapPoint && Player->CurrentCheckpoint == 3)
	{
		Player->CurrentGoalPoint += 1;
		Player->CurrentCheckpoint = 0;
		CurrentLapString = UKismetStringLibrary::Conv_IntToString(Player->CurrentGoalPoint);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, CurrentLapString);
	}
	else
	{
		return;
	}
}

void ALapVolume::Confirm_CheckPoint(AKartPlayer* Player)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT("CheckPoint"));

	if (!bIsThisLapPoint && bIsThisCheckPoint)
	{
		Player->CurrentCheckpoint += 1;
		CurrentCheckString = UKismetStringLibrary::Conv_IntToString(Player->CurrentCheckpoint);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, CurrentCheckString);
	}
	else
	{
		return;
	}
}

