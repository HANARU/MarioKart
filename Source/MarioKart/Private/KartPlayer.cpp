#include "KartPlayer.h"
#include "NinjaCharacter.h"
#include "NinjaCharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Camera/CameraComponent.h>

AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// kartbaseComp Scene컴포넌트
	kartbaseComp = CreateDefaultSubobject<USceneComponent>(TEXT("kartbaseComp"));
	kartbaseComp->SetupAttachment(GetMesh());

	// kartbaseComp 
	kartbaseComp->SetRelativeLocation(FVector(0, 0, -0));


	// kartmeshComp 컴포넌트 추가
	kartmeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartmeshComp"));
	kartmeshComp->SetupAttachment(kartbaseComp);
	kartmeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// kartmeshComp 위치, 크기

	// kartCamComp 컴포넌트 추가
	kartCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCamComp"));
	kartCamComp->SetupAttachment(kartbaseComp);

	// kartCamComp 위치, 크기
	kartCamComp->SetRelativeLocation(FVector(0, -200, 90));
	kartCamComp->SetRelativeRotation(FRotator(-20, 90, 0));
	
	// Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_body.SM_kart_body'"));

	if (TempMesh.Succeeded())
	{
		kartmeshComp->SetStaticMesh(TempMesh.Object);
	}
}
