// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioKartPlayerController.h"
#include "KartPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <DrawDebugHelpers.h>
#include <Camera/CameraShakeBase.h>
#include "Components/AudioComponent.h"



AMarioKartPlayerController::AMarioKartPlayerController()
{

	PrimaryActorTick.bCanEverTick = true;

	// 대쉬 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_.play_dash_'"));

	if (TempdashSound.Succeeded())
	{
		dashSound = TempdashSound.Object;
	}

	// 마리오 보이스 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempmariovoiceSound(TEXT("SoundWave'/Game/5_FX/Audio/play_mario-voice_.play_mario-voice_'"));

	if (TempmariovoiceSound.Succeeded())
	{
		mariovoiceSound = TempmariovoiceSound.Object;
	}
	
	// 출발 카운드 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempstartcountSound(TEXT("SoundWave'/Game/5_FX/Audio/play_Start-Sound-Effect_.play_Start-Sound-Effect_'"));

	if (TempstartcountSound.Succeeded())
	{
		startcountSound = TempstartcountSound.Object;
	}
}

void AMarioKartPlayerController::BeginPlay()
{
	// 플레이어 캐릭터 불러오기
	me = Cast<AKartPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 플레이어 기본 속도 설정
	me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	// 플레이어 확인 디버그 메시지
	FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);

	// 출발 카운트 사운드 재생
	if (startcountSound)
	{
		// soundbase 출발 카운트 사운드 오디오 컴포넌트 생성 및 초기화
		playingstartComp = UGameplayStatics::SpawnSound2D(GetWorld(), startcountSound);

		// 출발 카운트 사운드 유효성 검사
		if (playingstartComp)
		{
			playingstartComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
			playingstartComp->bAutoDestroy = true; // false : 재생 완료 후 자동으로 제거하지 않는다.

			// 출발 카운트 사운드 재생
			playingstartComp->Play();

		}
	}
	
}

void AMarioKartPlayerController::Tick(float DeltaTime)
{
	startcountTime += DeltaTime;

	if (startcountTime >= 3.7f) // 출발 카운드 사운드 재생 후 주행
	{
		UE_LOG(LogTemp, Warning, TEXT("startcountTime : %.2f"), startcountTime);
		// 전진, 후진 주행 (가속, 속도)
		MoveVertical();
	}

	if (bisMovingback == true)
	{
		// 후진할 때 이동
		me->AddMovementInput(Direction(), currentSpeed);
	}
	else
	{
		if (bisAcc == true)
		{
			if (bisJump == true)
			{
				// 드리프트 시작
				// 드리프트 시간 누적
				driftTime += DeltaTime;
				me->AddMovementInput(Direction(), currentSpeed);
				UE_LOG(LogTemp, Warning, TEXT("driftTime : %.2f"), driftTime);
				UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisJump ? true : false);

				// 드리프트가 3초이상 지속됐을 때
				if (driftTime >= 3.0f)
				{
					FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
					FString DriftString = FString::Printf(TEXT("drift 3"));
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
					// 3단 드리프트 무조건 발동
					DriftActivate(3.0f);
					UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisJump ? true : false);

					// 드리프트 시간 리셋
					driftTime = 0.0f;
				}
			}
			else
			{
				FVector Dir = Direction();

				if (bTestDebug)
					return;
				// 전진 주행 이동
				me->AddMovementInput(Direction(), currentSpeed);
				//UE_LOG(LogTemp, Warning, TEXT("currentSpeed : %.2f"), currentSpeed);
			}
		}
		else
		{
			// 가속키 누르지 않았을 때 서서히 감속
			me->AddMovementInput(Direction(), currentSpeed);
		}
	}
}

void AMarioKartPlayerController::SetupInputComponent()
{
	//입력 함수 바인딩
	Super::SetupInputComponent();

	// 좌우 입력
	InputComponent->BindAxis(TEXT("Horizontal"), this, &AMarioKartPlayerController::Horizontal);

	// 가속키 입력
	InputComponent->BindAction(TEXT("Acc"), IE_Pressed, this, &AMarioKartPlayerController::Acc);
	InputComponent->BindAction(TEXT("Acc"), IE_Released, this, &AMarioKartPlayerController::Acc_released);

	// 후진키 입력
	InputComponent->BindAction(TEXT("MoveBack"), IE_Pressed, this, &AMarioKartPlayerController::MoveBack);
	InputComponent->BindAction(TEXT("MoveBack"), IE_Released, this, &AMarioKartPlayerController::MoveBack_released);

	// 드리프트 (점프키 입력)
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMarioKartPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMarioKartPlayerController::Jump_released);

	// Direction 함수 디버그
	InputComponent->BindAction(TEXT("TestDebug"), IE_Pressed, this, &AMarioKartPlayerController::TestDebug);

	// 아이템 키 입력
	InputComponent->BindAction(TEXT("ItemUse"), IE_Pressed, this, &AMarioKartPlayerController::ItemUse);


}

void AMarioKartPlayerController::Acc()
{
	bisAcc = true;
	UE_LOG(LogTemp, Warning, TEXT("Acc: %d"), bisAcc ? true : false);
}

void AMarioKartPlayerController::Acc_released()
{
	bisAcc = false;
	UE_LOG(LogTemp, Warning, TEXT("Acc_released: %d"), bisAcc ? true : false);

}

void AMarioKartPlayerController::MoveBack()
{
	bisMovingback = true;
	UE_LOG(LogTemp, Warning, TEXT("MoveBack: %d"), bisMovingback ? true : false);
}

void AMarioKartPlayerController::MoveBack_released()
{
	bisMovingback = false;
	UE_LOG(LogTemp, Warning, TEXT("MoveBack_released: %d"), bisMovingback ? true : false);
}


void AMarioKartPlayerController::Jump()
{
	bisJump = true;
	UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisJump ? true : false);
}

void AMarioKartPlayerController::Jump_released()
{
	bisJump = false;

	
	if (driftTime >= 2.0f)
	{
		FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
		FString DriftString = FString::Printf(TEXT("drift 2"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
		// 드리프트가 2초이상 지속됐을 때
		// 2단 드리프트
		DriftActivate(2.0f);

		// 드리프트 해제
		driftTime = 0.0f;
	}
	else if (driftTime >= 1.0f)
	{
		FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
		FString DriftString = FString::Printf(TEXT("drift 1"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
		// 1단 드리프트
		DriftActivate(1.0f);

		// 드리프트 해제
		driftTime = 0.0f;

	}
	else
	{
		driftTime = 0.0f;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Jump_released: %d"), bisJump ? true : false);
	UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);
	UE_LOG(LogTemp, Warning, TEXT("DriftOff"));
}

void AMarioKartPlayerController::Horizontal(float value)
{
	// 좌우 입력값
	horizontalValue = value;

	// 좌우 입력 들어왔을 때(회전)
	if (FMath::Abs(horizontalValue) != 0.0f)
	{
		if (bisMovingback == true)
		{
			// 후진 회전
			float movigbackValue = FMath::Lerp(horizontalValue, 0.0f, 0.3f);
			AddYawInput(movigbackValue);
			UE_LOG(LogTemp, Warning, TEXT("movigbackValue : %.2f"), movigbackValue);

		}
		else
		{
			if (bisAcc == true)
			{
				if (bisJump == true)
				{
					// 드리프트 시작
					float driftValue = FMath::Lerp(0.0f, horizontalValue, 0.7f);
					AddYawInput(driftValue);
					UE_LOG(LogTemp, Warning, TEXT("driftValue : %.2f"), driftValue)
				}
				else
				{
					// 전진 회전
					AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
				}
			}
		}
	}

}

void AMarioKartPlayerController::MoveVertical()
{
	if (bisAcc == true || bisMovingback == true)
	{
		if (bisAcc == true)
		{
			// 전진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5);


		}
		else
		{
			// 후진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed * -1.8f, GetWorld()->GetDeltaSeconds());
		}
	}
	else
	{
		// 속도 0으로 조정
		currentSpeed = FMath::Lerp(currentSpeed, 0.0f, GetWorld()->GetDeltaSeconds() * 3.0f);

	}

}

// 이동 방향 벡터 반환 함수
FVector AMarioKartPlayerController::Direction()
{
	// 주행 방향 구하기
	FVector returnDirection = me->GetActorForwardVector();

	//UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());
	return returnDirection;
}

void AMarioKartPlayerController::DriftActivate(float dashActiveTime)
{
	// 플레이어 보이스 재생
	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);

	// 대쉬 사운드 재생
	if (dashSound)
	{
		// soundbase 대쉬 사운드 오디오 컴포넌트 생성 및 초기화
		playingAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), dashSound);

		// 대쉬 사운드 유효성 검사
		if (playingAudioComp)
		{
			playingAudioComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
			playingAudioComp->bAutoDestroy = false; // 재생 완료 후 자동으로 제거하지 않는다.

			// 대쉬 사운드 재생
			playingAudioComp->Play();
		}
	}	

	// 플레이어 이동 속도 2000 으로 늘어남
	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	// 드리프트 시간 동안 앞으로 빠르게 전진
	me->AddMovementInput(Direction(), currentSpeed);

	UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);
	UE_LOG(LogTemp, Warning, TEXT("DriftActivate: %.2f"), dashActiveTime);

	// 드리프트 지속 dashActiveTime초(타이머 사용)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
		if (me->GetCharacterMovement() != nullptr)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		}

		// 대쉬 사운드 멈추기대쉬 사운드
		if (playingAudioComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("STOP"));
 			playingAudioComp->Stop();
			playingAudioComp->SetActive(false);
			playingAudioComp = nullptr;
		}

		}), dashActiveTime, false);

	// 드리프트 해제
	driftTime = 0.0f;

}

void AMarioKartPlayerController::ItemUse()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemUse"));
	ItemActivate();
}

void AMarioKartPlayerController::ItemActivate()
{
	//// 대쉬 카메라 쉐이크
	//if (this->PlayerCameraManager != nullptr && dashShake != nullptr)
	//{
	//	
	//	this->PlayerCameraManager->StartCameraShake(dashShake);
	//	UE_LOG(LogTemp, Warning, TEXT("dashShake"));

	//}

	// 대쉬 속도
	me->GetCharacterMovement()->MaxWalkSpeed = 4000.0f;

	// 대쉬 1.8초 동안 지속
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([&]() {
		me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		}), 1.8f, false);
	UE_LOG(LogTemp, Warning, TEXT("ItemActivate"));

}


void AMarioKartPlayerController::TestDebug()
{
	bTestDebug = !bTestDebug;
}
