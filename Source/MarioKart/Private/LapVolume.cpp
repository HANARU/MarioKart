#include "LapVolume.h"
#include "GM_Race.h"
#include "KartPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

ALapVolume::ALapVolume()
{
	//PrimaryActorTick.bCanEverTick = true;
	CheckVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CheckVolume;
	CheckVolume->SetCollisionProfileName(FName(TEXT("ItemBox")));
}

void ALapVolume::BeginPlay()
{
	Super::BeginPlay();

	GMRace = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GetLocalRole() == ROLE_Authority)
	{
		CheckVolume->OnComponentBeginOverlap.AddDynamic(this, &ALapVolume::OnOverlapBegin);
	}
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
		Player->ReceiveFromLapVolume(bIsThisLapPoint, bIsThisCheckPoint);
	}
}