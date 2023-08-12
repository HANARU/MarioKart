
// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "GM_Race.h"
#include "Kismet/GameplayStatics.h"
#include "KartPlayer.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    BoxCollision->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	BoxCollision->bHiddenInGame = false;
    GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AKartPlayer* kartplayer = Cast<AKartPlayer>(OtherActor);
    

    if (kartplayer != nullptr)
    {
       
        int percent = FMath::RandRange(1, 100);
        FString ItemName;

        if (percent < 33)
        {
            ItemName = TEXT("Mush");
        }
        else if (percent < 66)
        {
            ItemName = TEXT("Coin");
        }
        else
        {
            ItemName = TEXT("Turtle");
        }

        kartplayer->Itemget = true;
        kartplayer->CollectItem(ItemName); 
        //Destroy
        
  
        }
    
}

