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
#include "MarioKartPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"



AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	UCapsuleComponent* CapsuleComp  = GetCapsuleComponent();
	SetRootComponent(CapsuleComp);

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

}

void AKartPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// 아이템 사용 처리등록
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AKartPlayer::Fire);
}

void AKartPlayer::Fire()
{
    if(Itemget == true)
	{ 
	if (CollectedItemName == TEXT("Mush"))
	{
		if (playerDashSound)
		{
			// soundbase 대쉬 사운드 오디오 컴포넌트 생성 및 초기화
			playingDashSound = UGameplayStatics::SpawnSound2D(GetWorld(), playerDashSound);

			// 대쉬 사운드 유효성 검사
			if (playingDashSound)
			{
				playingDashSound->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
				playingDashSound->bAutoDestroy = false; // 재생 완료 후 자동으로 제거하지 않는다.

				// 대쉬 사운드 재생
				playingDashSound->Play();
			}
		}
		GetCharacterMovement()->MaxWalkSpeed *= 5.0f;

		// 일정 시간(5초) 뒤에 속도를 원래 값으로 돌리기 위해 타이머를 설정합니다
		GetWorld()->GetTimerManager().SetTimer(SpeedResetTimerHandle, this, &AKartPlayer::ResetSpeedToNormal, 2.0f, false);

		Itemget = false;
	}
	else if (CollectedItemName == TEXT("Coin"))
	{
		GetCharacterMovement()->MaxWalkSpeed += 100.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed));

		Itemget = false;
	}
// 	else if (CollectedItemName == TEXT("Turtle"))
// 		{
// 			UClass* TurtleClass = AC_Turtle::StaticClass();
// 	
// 			// 캐릭터의 위치와 방향을 기반으로 액터를 스폰합니다
// 			FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.0f; // 적절한 위치 조정
// 			SpawnLocation.Z -= 90.0f; // z 좌표를 90만큼 낮춤
// 			FRotator SpawnRotation = GetActorRotation();
// 	
// 			// 액터를 스폰하고 생성된 액터를 가리키는 포인터를 받습니다
// 			AC_Turtle* SpawnedTurtle = GetWorld()->SpawnActor<AC_Turtle>(TurtleClass, SpawnLocation, SpawnRotation);
// 	
// 			SpawnedTurtle->SetLifeSpan(2.0f); // 1초 후에 액터가 파괴됩니다.
// 	
// 			Itemget = false;
// 		}
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

void AKartPlayer::CollectItem(FString ItemName)
{
	CollectedItemName = ItemName;

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, ItemName);
	// 아이템을 수집한 후에 필요한 추가 처리를 할 수 있습니다.
}
