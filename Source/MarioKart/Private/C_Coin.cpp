// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Coin.h"
#include "Components/BoxComponent.h"
#include "KartPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GM_Race.h"

// Sets default values
AC_Coin::AC_Coin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = BoxCollision;

    ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    ItemBoxMesh->SetRelativeLocation(FVector(3));
    ItemBoxMesh->SetupAttachment(RootComponent);
    ItemBoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    FVector NewLocation = FVector(120.0f, 0.0f, 0.0f);
    ItemBoxMesh->AddRelativeLocation(NewLocation);

    FVector NewScale = FVector(15.0f, 15.0f, 15.0f);
    ItemBoxMesh->SetWorldScale3D(NewScale);

    ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/3_SM/Stadium_Change/Item/Coin/SM_Item_Coin.SM_Item_Coin'"));
    if (MeshAsset.Succeeded())
    {
        ItemBoxMesh->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void AC_Coin::BeginPlay()
{
	Super::BeginPlay();
    
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AC_Coin::OnOverlapBegin);
    GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AC_Coin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Coin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AKartPlayer* kartplayer = Cast<AKartPlayer>(OtherActor);

    if (kartplayer != nullptr && GameMode != nullptr)
    {
        GameMode->ItemOverlaped(kartplayer);

        // 사운드를 재생합니다.
        USoundBase* CoinSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/5_FX/Audio/play_coin_sound_.play_coin_sound_'"));
        if (CoinSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, CoinSound, GetActorLocation());
        }

        // 플레이어의 이동속도를 0.25배 만큼 늘립니다.
        kartplayer->IncreaseSpeed();

        Destroy();
    }
}

