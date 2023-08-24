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
        Destroy();
    }
}

void AItemBase::ItemUpdate_Implementation(AKartPlayer* OwningPlayer)
{
    SetOwner(OwningPlayer);
}