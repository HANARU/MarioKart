#include "LapVolume.h"
#include "GM_Race.h"
#include "NinjaCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	CheckVolume->OnComponentBeginOverlap.AddDynamic(this, &ALapVolume::OnOverlapBegin);
}

// Called every frame
void ALapVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALapVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANinjaCharacter* Player = Cast<ANinjaCharacter>(OtherActor);
	if (Player != nullptr)
	{
		/*if (II_Interact* Interface = Cast<II_Interact>(Player))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Check"));
			Interface->Overlap_Lap_System(Player, bIsThisCheckPoint, bIsThisLapPoint);
		}*/
	}
}

