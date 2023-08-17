#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "GM_Race.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "KartPlayer.h"


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
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
    GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AKartPlayer* kartplayer = Cast<AKartPlayer>(OtherActor);
    

    if (kartplayer != nullptr && GameMode != nullptr)
    {
        GameMode->ItemOverlaped(kartplayer);
        Destroy();
    }
}

