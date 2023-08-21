// Fill out your copyright notice in the Description page of Project Settings.


#include "DecelerationVolume.h"
#include "GM_Race.h"
#include "KartPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADecelerationVolume::ADecelerationVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckDeceleration = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CheckDeceleration;
	CheckDeceleration->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ADecelerationVolume::BeginPlay()
{
	Super::BeginPlay();
	
	CheckDeceleration->OnComponentBeginOverlap.AddDynamic(this, &ADecelerationVolume::OnOverlapBegin);
	CheckDeceleration->OnComponentEndOverlap.AddDynamic(this, &ADecelerationVolume::OnOverlapEnd);
	GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ADecelerationVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADecelerationVolume::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AKartPlayer* kartplayer = Cast<AKartPlayer>(OtherActor);
	if (kartplayer != nullptr)
	{
		GameMode->ItemOverlaped(kartplayer);

		// 플레이어의 이동속도가 0.5배 만큼 감소한다.
		kartplayer->DecreaseSpeed();
	}
}

void ADecelerationVolume::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AKartPlayer* kartplayer = Cast<AKartPlayer>(OtherActor);
	if (kartplayer != nullptr)
	{
		GameMode->ItemOverlaped(kartplayer);

		// 플레이어의 이동속도를 다시 복구시킨다.
		kartplayer->ReturnSpeed();
	}
}

