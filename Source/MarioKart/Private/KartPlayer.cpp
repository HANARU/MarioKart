#include "KartPlayer.h"
#include "NinjaCharacter.h"
#include "NinjaCharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include <Camera/CameraComponent.h>
#include "GM_Race.h"
#include <GameFramework/SpringArmComponent.h>
#include "MarioKartPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/AudioComponent.h"
#include "KartPlayerAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "ItemComponent.h"


AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* CapsuleComp  = GetCapsuleComponent();
	SetRootComponent(CapsuleComp);

	// self �Ӽ�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//charactermovement �Ӽ�
	GetCharacterMovement()->bOrientRotationToMovement = false;

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

	// kartmeshComp Mesh ������ �Ҵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_body.SM_kart_body'"));

	if (TempMesh.Succeeded())
	{
		kartmeshComp->SetStaticMesh(TempMesh.Object);
	}

	// kartwheelComp ������Ʈ �߰�
	kartwheelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartwheelComp"));
	kartwheelComp->SetupAttachment(kartbaseSceneComp);
	kartwheelComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// kartwheelComp Mesh ������ �Ҵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempwheelMesh(TEXT("StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_Wheel.SM_kart_Wheel'"));

	if (TempwheelMesh.Succeeded())
	{
		kartwheelComp->SetStaticMesh(TempwheelMesh.Object);
	}

	// kartwheelComp ũ��
	kartwheelComp->SetRelativeScale3D(FVector(1.5f));

	// kartCharacterBody ������Ʈ �߰�
	kartCharacterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("kartCharacterBody"));
	kartCharacterBody->SetupAttachment(kartbaseSceneComp);

	// kartCharacterBody Mesh ������ �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempCharacterMesh(TEXT("SkeletalMesh'/Game/4_SK/Mario/Mesh/Mario.Mario'"));

	if (TempCharacterMesh.Succeeded())
	{
		kartCharacterBody->SetSkeletalMesh(TempCharacterMesh.Object);
		kartCharacterBody->SetRelativeLocation(FVector(0, -10, -20));
		kartCharacterBody->SetRelativeScale3D(FVector(5));
	}

	// kartSpringComp ������Ʈ �߰�
	kartSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("kartSpringArmComp"));
	kartSpringComp->SetupAttachment(kartbaseSceneComp);

	// kartSpringComp ��ġ, ũ��
	kartSpringComp->SetRelativeLocation(FVector(0, 0, 50));
	kartSpringComp->SetRelativeRotation(FRotator(-15, 90, 0));
	kartSpringComp->TargetArmLength=350;
	kartSpringComp->bEnableCameraLag = true;
	kartSpringComp->bEnableCameraRotationLag = true;

	// kartCamComp ������Ʈ �߰�
	kartCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCamComp"));
	kartCamComp->SetupAttachment(kartSpringComp);
	
	// kartCamComp ��ġ, ũ��
	kartCamComp->SetRelativeRotation(FRotator(5, 0, 0));

	// �뽬 ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempkartdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_.play_dash_'"));

	if (TempkartdashSound.Succeeded())
	{
		playerDashSound = TempkartdashSound.Object;
	}


	bReplicates = true;
	//bReplicateMovement = true;
	SetReplicateMovement(true);

	// kartItem ������Ʈ �߰�
	playerItemComp = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));


}

void AKartPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// ������ ��� ó�����
	PlayerInputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &AKartPlayer::UsingItem);
}

void AKartPlayer::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->ItemSignature.BindUObject(this, &AKartPlayer::ReceiveItem);
	}
}

void AKartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Item1stString);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, Item2ndString);


}

void AKartPlayer::ReceiveItem(int32 ItemNum)
{	
	//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Hello"));
	playerItemComp->ReceiveItemfromCharacter(ItemNum);

}

void AKartPlayer::UsingItem()
{
	playerItemComp->UsingItem();
	
}

void AKartPlayer::ResetSpeedToNormal()
{
	// 5�� �Ŀ� �ӵ��� ���� ������ �����մϴ�
	GetCharacterMovement()->MaxWalkSpeed /= 5.0f;

	if (playingDashSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("STOP"));
		playingDashSound->Stop();
		playingDashSound->SetActive(false);
		playingDashSound = nullptr;
	}

	// Ÿ�̸� �ڵ��� ��ȿȭ�մϴ�
	SpeedResetTimerHandle.Invalidate();
}

//void AKartPlayer::PlayAnimationMontage()
//{
//	if (horizontalValue)
//	{
//		if (horizontalValue == 1.0f)
//		{
//			UAnimInstance* animInstance = Mesh
//		}
//	}
//}

// horizontalvalue ���� ����ȭ�� ���� ����� �� ����Ǵ� �Լ�
//void AKartPlayer::OnRep_Horizontal()
//{
//	if (horizontalValue != 0 && HasAuthority())
//	{
//		// ������ �ִϸ��̼� �� ���� ��û
//		ServerOnRep_Horizontal();
//	}
//	UKartPlayerAnimInstance* anim = Cast<UKartPlayerAnimInstance>(GetMesh()->GetAnimInstance());
//	
//	if (anim != nullptr)
//	{
//		anim->HorizontalValue = horizontalValue;
//	}
//
//	
//}

void AKartPlayer::ServerHorizontal_Implementation()
{
	//MulticastOnRep_Horizontal();

	UKartPlayerAnimInstance* anim = Cast<UKartPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (anim != nullptr)
	{
		anim->HorizontalValue = horizontalValue;
	}
}

//void AKartPlayer::MulticastOnRep_Horizontal_Implementation()
//{
//	UKartPlayerAnimInstance* anim = Cast<UKartPlayerAnimInstance>(GetMesh()->GetAnimInstance());
//
//	if (anim != nullptr)
//	{
//		anim->HorizontalValue = horizontalValue;
//	}
//
//}

void AKartPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKartPlayer, horizontalValue);
	//DOREPLIFETIME(AKartPlayer, this);
}