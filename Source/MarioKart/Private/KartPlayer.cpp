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
#include "State_KartPlayer.h"
#include "GameFramework/GameState.h"
#include "KartInstance.h"
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

	MeshArray.SetNum(4);

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
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh_Mario(TEXT("SkeletalMesh'/Game/4_SK/Mario/Mesh/Mario.Mario'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh_Luige(TEXT("SkeletalMesh'/Game/4_SK/Luige/Mesh/Luige.Luige'"));

	if (Mesh_Mario.Succeeded())
	{
		SK_Mario = Mesh_Mario.Object;
		//kartCharacterBody->SetSkeletalMesh(Mesh_Mario.Object);
		//kartCharacterBody->SetRelativeLocation(FVector(0, -10, -20));
		//kartCharacterBody->SetRelativeScale3D(FVector(5));
		//kartCharacterBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Mesh_Luige.Succeeded())
	{
		SK_Luige = Mesh_Luige.Object;
	}

	// parachute 컴포넌트 추가
	kartParachute = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("kartParachute"));
	kartParachute->SetupAttachment(kartbaseSceneComp);
	kartParachute->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	kartParachute->SetVisibility(false);

	// kartParachute Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempParachute(TEXT("StaticMesh'/Game/3_SM/Object/Parachute/parachute.parachute'"));

	if (TempParachute.Succeeded())
	{
		kartParachute->SetStaticMesh(TempParachute.Object);
	}

	// kartParachute 위치, 크기
	kartParachute->SetRelativeLocation(FVector(0, -35, 18));
	kartParachute->SetRelativeRotation(FRotator(0, 0, 80));
	kartParachute->SetRelativeScale3D(FVector(5));

	// kartSpringComp 컴포넌트 추가
	kartSpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("kartSpringArmComp"));
	kartSpringComp->SetupAttachment(kartbaseSceneComp);

	// kartSpringComp 위치, 크기
	kartSpringComp->SetRelativeLocation(FVector(0, 0, 50));
	kartSpringComp->SetRelativeRotation(FRotator(-15, 90, 0));
	kartSpringComp->TargetArmLength=350;
	kartSpringComp->bEnableCameraLag = true;
	kartSpringComp->bEnableCameraRotationLag = true;

	// kartCameraComp 컴포넌트 추가
	kartCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("kartCameraComp"));
	kartCameraComp->SetupAttachment(kartSpringComp);
	
	// kartCamComp 위치, 크기
	kartCameraComp->SetRelativeRotation(FRotator(5, 0, 0));

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

	// 동기화 설정
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

	FTimerHandle DelayHandle;

	/*GetWorldTimerManager().SetTimer(DelayHandle,FTimerDelegate::CreateLambda([this]() 
		{
			TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;

			for (int index = 0; index < players.Num(); ++index)
			{
				KartPlayerState = Cast<AState_KartPlayer>(players[index]);
				if (KartPlayerState != nullptr)
				{
					KartPlayerState->PlayerNum = PlayerNumber;
				}
				UE_LOG(LogTemp, Warning, TEXT("Player : %d"), KartPlayerState != nullptr ? KartPlayerState->PlayerNum : 12);
			}

		}), 10.f, false);*/

	KartInstance = Cast<UKartInstance>(GetGameInstance());

	PlayerNumber = KartInstance->CurrentPlayerNum;

	

	UE_LOG(LogTemp, Warning, TEXT("PlayerState Check End"));

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

	
}


void AKartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//PrintStringAtPlayer();

	if (!HasAuthority())
	{
		ServerHorizontal_Implementation();
	}

	if (KartInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not NULL"));
		switch (PlayerNumber)
		{
		case 0:			// 첫번째 플레이어 = 마리오
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has 0 num"));
			kartCharacterBody->SetSkeletalMesh(SK_Mario);
			break;
		}
		case 1:			// 두번째 플레이어 = 루이지
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has 1 num"));
			kartCharacterBody->SetSkeletalMesh(SK_Luige);
			break;
		}
		case 2:
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has 2 num"));
			break;
		}
		case 3:			// 네번째 플레이어 = 요시
		{
			UE_LOG(LogTemp, Warning, TEXT("Player has 3 num"));
			break;
		}
		default:
			UE_LOG(LogTemp, Warning, TEXT("NULL Detected"));
			break;
		}
	}
	//PlayAnimationMontage();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, kart);
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
	if (KartPlayerState != nullptr)
	{
		KartPlayerState->UseItem();
	}
	/*if (Current1stItem == NoItem && Current2ndItem == NoItem)
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
	}*/
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