#include "KartPlayer.h"
#include "NinjaCharacter.h"
#include "NinjaCharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include <Camera/CameraComponent.h>
#include "C_Turtle.h"
#include "GM_Race.h"
#include <GameFramework/SpringArmComponent.h>

AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	UCapsuleComponent* CapsuleComp  = GetCapsuleComponent();
	SetRootComponent(CapsuleComp);

	// kartbaseComp Scene������Ʈ
	kartbaseSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	kartbaseSceneComp->SetupAttachment(CapsuleComp);

	// kartbaseComp 
	kartbaseSceneComp->SetRelativeLocation(FVector(0, 0, -70));
	kartbaseSceneComp->SetRelativeRotation(FRotator(0, -90, 0));

	// kartmeshComp ������Ʈ �߰�
	kartmeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartmeshComp"));
	kartmeshComp->SetupAttachment(kartbaseSceneComp);
	kartmeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// kartmeshComp ũ��
	kartmeshComp->SetRelativeScale3D(FVector(1.5f));

	// kartCharacterBody ������Ʈ �߰�
	kartCharacterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("kartCharacterBody"));
	kartCharacterBody->SetupAttachment(kartbaseSceneComp);
	kartCharacterBody->SetRelativeLocation(FVector(0, 0, -20));
	kartCharacterBody->SetRelativeRotation(FRotator(0, 0, 0));
	kartCharacterBody->SetRelativeScale3D(FVector(4));

	// kartSpringComp ������Ʈ �߰�
	kartSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("kartSpringArmComp"));
	kartSpringComp->SetupAttachment(kartbaseSceneComp);

	// kartSpringComp ��ġ, ũ��
	kartSpringComp->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	kartSpringComp->SetRelativeRotation(FRotator(-10.0f, 90.0f, 0.0f));
	kartSpringComp->TargetArmLength=250.0f;

	// kartCamComp ������Ʈ �߰�
	kartCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCamComp"));
	kartCamComp->SetupAttachment(kartSpringComp);

	//// kartCamComp ��ġ, ũ��
	//kartCamComp->SetRelativeLocation(FVector(0.0f, -200.0f, 90.0f));
	//kartCamComp->SetRelativeRotation(FRotator(-10.0f, 90.0f, 0.0f));
	
	// Mesh ������ �Ҵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_body.SM_kart_body'"));

	if (TempMesh.Succeeded())
	{
		kartmeshComp->SetStaticMesh(TempMesh.Object);
	}

}

void AKartPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// ������ ��� ó�����
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AKartPlayer::Fire);
}

void AKartPlayer::Fire()
{
    if(Itemget == true)
	{ 
	if (CollectedItemName == TEXT("Mush"))
	{
		GetCharacterMovement()->MaxWalkSpeed *= 5.0f;

		// ���� �ð�(5��) �ڿ� �ӵ��� ���� ������ ������ ���� Ÿ�̸Ӹ� �����մϴ�
		GetWorld()->GetTimerManager().SetTimer(SpeedResetTimerHandle, this, &AKartPlayer::ResetSpeedToNormal, 2.0f, false);

		Itemget = false;
	}
	else if (CollectedItemName == TEXT("Coin"))
	{
		GetCharacterMovement()->MaxWalkSpeed += 100.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed));

		Itemget = false;
	}
	else if (CollectedItemName == TEXT("Turtle"))
	{
		UClass* TurtleClass = AC_Turtle::StaticClass();

		// ĳ������ ��ġ�� ������ ������� ���͸� �����մϴ�
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.0f; // ������ ��ġ ����
		SpawnLocation.Z -= 90.0f; // z ��ǥ�� 90��ŭ ����
		FRotator SpawnRotation = GetActorRotation();

		// ���͸� �����ϰ� ������ ���͸� ����Ű�� �����͸� �޽��ϴ�
		AC_Turtle* SpawnedTurtle = GetWorld()->SpawnActor<AC_Turtle>(TurtleClass, SpawnLocation, SpawnRotation);

		SpawnedTurtle->SetLifeSpan(2.0f); // 1�� �Ŀ� ���Ͱ� �ı��˴ϴ�.

		Itemget = false;
	}
	}
}


void AKartPlayer::ResetSpeedToNormal()
{
	// 5�� �Ŀ� �ӵ��� ���� ������ �����մϴ�
	GetCharacterMovement()->MaxWalkSpeed /= 5.0f;

	// Ÿ�̸� �ڵ��� ��ȿȭ�մϴ�
	SpeedResetTimerHandle.Invalidate();
}

void AKartPlayer::CollectItem(FString ItemName)
{
	CollectedItemName = ItemName;

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, ItemName);
	// �������� ������ �Ŀ� �ʿ��� �߰� ó���� �� �� �ֽ��ϴ�.
}
