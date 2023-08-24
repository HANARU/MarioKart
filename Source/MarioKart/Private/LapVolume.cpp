#include "LapVolume.h"
#include "GM_Race.h"
#include "KartPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

ALapVolume::ALapVolume()
{
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

void ALapVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Overlapped"));
	if (const auto Character = Cast<AKartPlayer>(OtherActor))
	{
		LapUpdate(Character);
	}
}

void ALapVolume::CheckLapData()
{
	if (bIsThisCheckPoint)		// CheckPoint Add
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("CheckPoint Overlapped"));
		LapCheckValue = 8;
		return;
	}
	if (bIsThisLapPoint)		// LapPoint Add
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("LapPoint Overlapped"));
		LapCheckValue = 9;
		return;
	}
}

void ALapVolume::LapUpdate_Implementation(AKartPlayer* OwningPlayer)
{
	SetOwner(OwningPlayer);
}
