#include "OverlayActor.h"
#include "Components/SpotLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"

#define Mario 0
#define Luige 1

AOverlayActor::AOverlayActor()
{
	PrimaryActorTick.bCanEverTick = true;
	 
	BaseScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = BaseScene;

	KartBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Kart"));
	KartBody->SetupAttachment(RootComponent);
	KartBody->SetRelativeRotation(FRotator(0,-90,0));

	PlayerBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player"));
	PlayerBody->SetupAttachment(RootComponent);
	PlayerBody->SetRelativeLocation(FVector(-10, 0, -20));
	PlayerBody->SetRelativeRotation(FRotator(0, -90, 0));
	PlayerBody->SetRelativeScale3D(FVector(4));

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	Capture->SetupAttachment(RootComponent);
	Capture->SetRelativeLocation(FVector(52.315994, -18.001703, 12.547211));
	Capture->SetRelativeRotation(FRotator(-10, 150, 0));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(RootComponent);
	Light->SetRelativeLocation(FVector(0, 0, 180));
	Light->SetRelativeRotation(FRotator(-90, 0, 0));
}

void AOverlayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOverlayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

