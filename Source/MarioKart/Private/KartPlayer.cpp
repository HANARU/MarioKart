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

	// self 속성
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//charactermovement 속성
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// kartbaseComp Scene컴포넌트
	kartbaseSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	kartbaseSceneComp->SetupAttachment(CapsuleComp);

	// kartbaseComp 
	kartbaseSceneComp->SetRelativeLocation(FVector(0, 0, -70));
	kartbaseSceneComp->SetRelativeRotation(FRotator(0, -90, 0));

	// kartmeshComp 컴포넌트 추가
	kartmeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartmeshComp"));
	kartmeshComp->SetupAttachment(kartbaseSceneComp);
	kartmeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// kartmeshComp 크기
	kartmeshComp->SetRelativeScale3D(FVector(1.5f));

	// kartmeshComp Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_body.SM_kart_body'"));

	if (TempMesh.Succeeded())
	{
		kartmeshComp->SetStaticMesh(TempMesh.Object);
	}

	// kartwheelComp 컴포넌트 추가
	kartwheelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartwheelComp"));
	kartwheelComp->SetupAttachment(kartbaseSceneComp);
	kartwheelComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// kartwheelComp Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempwheelMesh(TEXT("StaticMesh'/Game/3_SM/Object/Kart_Body1/SM_kart_Wheel.SM_kart_Wheel'"));

	if (TempwheelMesh.Succeeded())
	{
		kartwheelComp->SetStaticMesh(TempwheelMesh.Object);
	}

	// kartwheelComp 크기
	kartwheelComp->SetRelativeScale3D(FVector(1.5f));

	// kartCharacterBody 컴포넌트 추가
	kartCharacterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("kartCharacterBody"));
	kartCharacterBody->SetupAttachment(kartbaseSceneComp);

	// kartCharacterBody Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempCharacterMesh(TEXT("SkeletalMesh'/Game/4_SK/Mario/Mesh/Mario.Mario'"));

	if (TempCharacterMesh.Succeeded())
	{
		kartCharacterBody->SetSkeletalMesh(TempCharacterMesh.Object);
		kartCharacterBody->SetRelativeLocation(FVector(0, -10, -20));
		kartCharacterBody->SetRelativeScale3D(FVector(5));
	}

	// kartSpringComp 컴포넌트 추가
	kartSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("kartSpringArmComp"));
	kartSpringComp->SetupAttachment(kartbaseSceneComp);

	// kartSpringComp 위치, 크기
	kartSpringComp->SetRelativeLocation(FVector(0, 0, 50));
	kartSpringComp->SetRelativeRotation(FRotator(-15, 90, 0));
	kartSpringComp->TargetArmLength=350;
	kartSpringComp->bEnableCameraLag = true;
	kartSpringComp->bEnableCameraRotationLag = true;

	// kartCamComp 컴포넌트 추가
	kartCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCamComp"));
	kartCamComp->SetupAttachment(kartSpringComp);
	
	// kartCamComp 위치, 크기
	kartCamComp->SetRelativeRotation(FRotator(5, 0, 0));

	// 대쉬 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempkartdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_.play_dash_'"));

	if (TempkartdashSound.Succeeded())
	{
		playerDashSound = TempkartdashSound.Object;
	}

	// kartItem 컴포넌트 추가
	playerItemComp = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));

	// 플레이어 기본 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 1300.0f;

	// 플레이어 애니메이션 블루프린트
	kartCharacterBody->SetAnimInstanceClass(UKartPlayerAnimInstance::StaticClass());

	bReplicates = true;
	//bReplicateMovement = true;
	SetReplicateMovement(true);

}

void AKartPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// 아이템 사용 처리등록
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

	if (kartCharacterBody)
	{
		anim = Cast<UKartPlayerAnimInstance>(kartCharacterBody->GetAnimInstance());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::Red, TEXT(";("));

	}
}

void AKartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Item1stString);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Float Value: %.2f"), horizontalValue));
	
	if (!HasAuthority())
	{
		ServerHorizontal_Implementation();
	}

// 	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Item1stString);
// 	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, Item2ndString);

	PlayAnimationMontage();
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
	// 5초 후에 속도를 원래 값으로 리셋합니다
	GetCharacterMovement()->MaxWalkSpeed /= 5.0f;

	if (playingDashSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("STOP"));
		playingDashSound->Stop();
		playingDashSound->SetActive(false);
		playingDashSound = nullptr;
	}

	// 타이머 핸들을 무효화합니다
	SpeedResetTimerHandle.Invalidate();
}

void AKartPlayer::IncreaseSpeed()
{
	// 현재 이동속도를 가져옵니다.
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// 이동속도를 0.25배 만큼 늘립니다.
	float NewSpeed = CurrentSpeed * 1.05f;

	// 최대 이동속도를 제한합니다 (1.5배까지만 가능하도록).
	float MaxAllowedSpeed = 2000;
	if (NewSpeed > MaxAllowedSpeed)
	{
		NewSpeed = MaxAllowedSpeed;
	}

	// 새로운 이동속도를 적용합니다.
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	// 디버그 메시지로 새로운 이동속도를 출력합니다.
	FString DebugMessage = FString::Printf(TEXT("Speed : %.2f"), NewSpeed);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
}

void AKartPlayer::DecreaseSpeed()
{
	// 현재 이동속도를 가져옵니다.
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// 이동속도를 반으로 감소시킨다.
	float Despeed = CurrentSpeed * 0.5f;

	// 새로운 이동속도를 적용합니다.
	GetCharacterMovement()->MaxWalkSpeed = Despeed;

	// 디버그 메시지로 새로운 이동속도를 출력합니다.
	FString DebugMessage = FString::Printf(TEXT("Speed : %.2f"), Despeed);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);

}

void AKartPlayer::ReturnSpeed()
{
	// 현재 이동속도를 가져옵니다.
	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	// 이동속도를 반으로 감소시킨다.
	float speed = CurrentSpeed * 2.0f;

	// 새로운 이동속도를 적용합니다.
	GetCharacterMovement()->MaxWalkSpeed = speed;

	// 디버그 메시지로 새로운 이동속도를 출력합니다.
	FString DebugMessage = FString::Printf(TEXT("Speed : %.2f"), speed);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
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

void AKartPlayer::PlayAnimationMontage()
{
	if (horizontalValue)
	{
		if (horizontalValue == 1.0f)
		{
			if (M_Right)
			{
				PlayAnimMontage(M_Right, 1, NAME_None);
			}
		}
		else if (horizontalValue == -1.0f)
		{
			if (M_Left)
			{
				PlayAnimMontage(M_Left, 1, NAME_None);
			}
		}
		else
		{
			if (M_Base)
			{
				PlayAnimMontage(M_Base, 1, NAME_None);
			}
		}
	}
}

// horizontalvalue 값이 동기화로 인해 변경될 때 실행되는 함수
//void AKartPlayer::OnRep_Horizontal()
//{
//	if (horizontalValue != 0 && HasAuthority())
//	{
//		// 서버에 애니메이션 값 대입 요청
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

	/*if (anim != nullptr)
	{
		anim->HorizontalValue = horizontalValue;
	}*/
	MulticastHorizontal();
}

void AKartPlayer::MulticastHorizontal_Implementation()
{
	anim->HorizontalValue = horizontalValue;
	//anim->OnRep_HorizontalValue();
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("server Value: %.2f"), horizontalValue));
}

void AKartPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKartPlayer, horizontalValue);
	//DOREPLIFETIME(AKartPlayer, this);
}