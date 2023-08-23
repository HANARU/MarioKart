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
#include "Widget_Player.h"
#include "Net/UnrealNetwork.h"
#include "ItemComponent.h"
#include "DrawDebugHelpers.h"

#define NoItem 12


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

	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		UI_PlayerInGame = CreateWidget<UWidget_Player>(GetWorld(), InGamePlayerWidget);
		if (UI_PlayerInGame != nullptr)
		{
			UI_PlayerInGame->AddToViewport();
		}
	}
	LocalItemDataMessage = FString::Printf(TEXT("Your First Item : %d, Second Item : %d"), Current1stItem, Current2ndItem);
	ServerLapDataMessage = FString::Printf(TEXT("%s now has Lap %d, Checkpoint %d"), *GetFName().ToString(), CurrentGoalPoint, CurrentCheckPoint);
}


void AKartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PrintStringAtPlayer();

	if (!HasAuthority())
	{
		ServerHorizontal_Implementation();
	}
	//PlayAnimationMontage();
}

void AKartPlayer::OnRep_CurrentItemData()
{
	OnCurrentItemDataUpdate();
}

void AKartPlayer::PrintStringAtPlayer()
{
	FString CombineString;
	CombineString.Append(LocalItemDataMessage);
	CombineString.Append("\n");
	CombineString.Append(ServerLapDataMessage);

	DrawDebugString(GetWorld(), GetActorLocation(), CombineString, nullptr, FColor::White, 0, true);
}

void AKartPlayer::OnCurrentItemDataUpdate()
{
	if (IsLocallyControlled())
	{
		if (ItemDataLog)
		{
			LocalItemDataMessage = FString::Printf(TEXT("Your First Item : %d, Second Item : %d"), Current1stItem, Current2ndItem);
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		ServerItemDataMessage = FString::Printf(TEXT("%s's First Item : %d, Second Item : %d"), *GetFName().ToString(), Current1stItem, Current2ndItem);
		if (ItemDataLog)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ServerItemDataMessage);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, ServerItemDataMessage);
		}
	}
}

void AKartPlayer::ReceiveItem(int32 ItemNum)
{	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (Current1stItem == NoItem && Current2ndItem == NoItem)
		{
			Current1stItem = ItemNum;
		}
		else if (Current1stItem != NoItem && Current2ndItem == NoItem)
		{
			Current2ndItem = Current1stItem;
			Current1stItem = ItemNum;
		}
	}
	OnCurrentItemDataUpdate();
}

void AKartPlayer::UsingItem()
{
	if (Current1stItem == NoItem && Current2ndItem == NoItem)
	{
		return;
	}
	if (Current2ndItem == NoItem)
	{
		Current1stItem = NoItem;
		LocalItemDataMessage = FString::Printf(TEXT("Your First Item : %d, Second Item : %d"), Current1stItem, Current2ndItem);
	}
	if (Current2ndItem != NoItem)
	{
		Current1stItem = Current2ndItem;
		Current2ndItem = NoItem;
		LocalItemDataMessage = FString::Printf(TEXT("Your First Item : %d, Second Item : %d"), Current1stItem, Current2ndItem);
	}
}

void AKartPlayer::Multicast_PlayAnimation_Implementation()
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

void AKartPlayer::OnRep_CurrentLapdata()
{
	OnCurrentLapDataUpdate();
}

void AKartPlayer::ReceiveFromLapVolume(bool IsThisGoalPoint, bool IsThisCheckPoint)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (IsThisGoalPoint && !IsThisCheckPoint)
		{
			CurrentGoalPoint += 1;
			CurrentCheckPoint = 0;
			ServerLapDataMessage = FString::Printf(TEXT("%s now has Lap %d, Checkpoint %d"), *GetFName().ToString(), CurrentGoalPoint, CurrentCheckPoint);
		}
		else if (!IsThisGoalPoint && IsThisCheckPoint)
		{
			CurrentCheckPoint += 1;
			ServerLapDataMessage = FString::Printf(TEXT("%s now has Lap %d, Checkpoint %d"), *GetFName().ToString(), CurrentGoalPoint, CurrentCheckPoint);
		}

		OnCurrentLapDataUpdate();
	}
}

void AKartPlayer::OnCurrentLapDataUpdate()
{
	if (IsLocallyControlled())
	{
		LocalLapDataMessage = FString::Printf(TEXT("Your Lap is %d, Your Checkpoint is %d."), CurrentGoalPoint, CurrentCheckPoint);
		if (LapDataLog)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, LocalLapDataMessage);
			
		}
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		ServerLapDataMessage = FString::Printf(TEXT("%s now has Lap %d, Checkpoint %d"), *GetFName().ToString(), CurrentGoalPoint, CurrentCheckPoint);
		if (LapDataLog)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ServerLapDataMessage);
			if (CurrentGoalPoint == 2)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Game Over by %s"), *GetFName().ToString()));
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
	Multicast_PlayAnimation_Implementation();
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
	DOREPLIFETIME(AKartPlayer, Current1stItem);
	DOREPLIFETIME(AKartPlayer, Current2ndItem);
}