#include "ItemBase.h"
#include "GM_Race.h"
#include "KartPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

AItemBase::AItemBase()
{
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = BoxCollision;
    BoxCollision->bHiddenInGame = false;

    ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    ItemBoxMesh->SetRelativeLocation(FVector(3));
    ItemBoxMesh->SetupAttachment(RootComponent);
    ItemBoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ItemBoxMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mark"));
    ItemBoxMark->SetRelativeLocation(FVector(1.5));
    ItemBoxMark->SetupAttachment(RootComponent);
    ItemBoxMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    bReplicates = true;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
    GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
    
}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (const auto Character = Cast<AKartPlayer>(OtherActor))
    {
        ItemUpdate(Character);
    }
}

void AItemBase::CheckItemData()
{
    CheckItemGotNow = FMath::RandRange(0, 3);
}


void AItemBase::ItemUpdate_Implementation(AKartPlayer* OwningPlayer)
{
    SetOwner(OwningPlayer);
}

void AItemBase::SendItemByOverlap()
{
    //this->ItemSignature.ExecuteIfBound();
}

//AKartPlayer* Player = Cast<AKartPlayer>(OtherActor);

//if (Player != nullptr && GameMode != nullptr)
//{
//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s is overlaped."), *Player->GetName()));
//    //GameMode->ItemOverlaped(Player);
//    Player->ReceiveItem(FMath::RandRange(0, 9));
//    Destroy();
//}