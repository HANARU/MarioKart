#include "KartPlayer.h"
#include "NinjaCharacter.h"
#include "NinjaCharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Camera/CameraComponent.h>

AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// kartbaseComp Scene������Ʈ
	kartbaseComp = CreateDefaultSubobject<USceneComponent>(TEXT("kartbaseComp"));
	kartbaseComp->SetupAttachment(GetMesh());

	// kartbaseComp 
	kartbaseComp->SetRelativeLocation(FVector(0, 0, -0));


	// kartmeshComp ������Ʈ �߰�
	kartmeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartmeshComp"));
	kartmeshComp->SetupAttachment(kartbaseComp);
	kartmeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// kartmeshComp ��ġ, ũ��

	// kartCamComp ������Ʈ �߰�
	kartCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCamComp"));
	kartCamComp->SetupAttachment(kartbaseComp);

	// kartCamComp ��ġ, ũ��
	kartCamComp->SetRelativeLocation(FVector(0, -200, 90));
	kartCamComp->SetRelativeRotation(FRotator(-20, 90, 0));
	
	// Mesh ������ �Ҵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_body.SM_kart_body'"));

	if (TempMesh.Succeeded())
	{
		kartmeshComp->SetStaticMesh(TempMesh.Object);
	}
}
