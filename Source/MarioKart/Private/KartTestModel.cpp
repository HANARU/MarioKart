#include "KartTestModel.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

AKartTestModel::AKartTestModel()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	BaseScene->SetupAttachment(GetCapsuleComponent());
	BaseScene->SetRelativeLocation(FVector(0, 0, -70));

	KartBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KartBody"));
	KartBody->SetupAttachment(BaseScene);
	KartBody->SetRelativeRotation(FRotator(0, -90, 0));

	CharacterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player"));
	CharacterBody->SetupAttachment(BaseScene);
	CharacterBody->SetRelativeLocation(FVector(-5, 0, -20));
	CharacterBody->SetRelativeRotation(FRotator(0, -90, 0));
	CharacterBody->SetRelativeScale3D(FVector(4));

	TestCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TestCamera->SetupAttachment(BaseScene);
	TestCamera->SetRelativeLocation(FVector(-150, 0, 60));
	TestCamera->SetRelativeRotation(FRotator(-10, 0, 0));
}

void AKartTestModel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKartTestModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKartTestModel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AKartTestModel::Horizontal);

}

void AKartTestModel::Horizontal(float Value)
{
	HorizontalValue = Value;
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("%f"), HorizontalValue));
}

