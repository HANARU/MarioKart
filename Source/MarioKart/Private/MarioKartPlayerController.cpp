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
#include <Camera/CameraComponent.h>
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "Net/UnrealNetwork.h" // 언리얼 네트워크 기능
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "KartPlayerAnimInstance.h"
#include "GM_Race.h"




AMarioKartPlayerController::AMarioKartPlayerController()
{

	PrimaryActorTick.bCanEverTick = true;

	// 기본값으로 PlayerID를 0으로 설정
	//PlayerID = 0;

	

	// 주행 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempdriveSound(TEXT("SoundWave'/Game/5_FX/Audio/play_drive_kart.play_drive_kart'"));

	if (TempdriveSound.Succeeded())
	{
		driveSound = TempdriveSound.Object;
	}

	// 대쉬 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_short.play_dash_short'"));

	if (TempdashSound.Succeeded())
	{
		dashSound = TempdashSound.Object;
	}

	// 드리프트 사운드 가져오기
	ConstructorHelpers::FObjectFinder<USoundBase> TempdriftSound(TEXT("SoundWave'/Game/5_FX/Audio/play_DRIFT_STEER.play_DRIFT_STEER'"));

	if (TempdriftSound.Succeeded())
	{
		driftSound = TempdriftSound.Object;
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

	// 타임라인
	//driftjumpCurve = nullptr;

	// 변수 복제 기능 사용
	bReplicates = true;
	SetReplicateMovement(true);
}

void AMarioKartPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임모드
	RaceGM = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
		

	//if (HasAuthority())
	//{
	//	gm = GetWorld()->GetAuthGameMode<AGM_Race>();
	//}

	//FActorSpawnParameters param;
	//param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//me = GetWorld()->SpawnActor<AKartPlayer>(kartPlayer, param);	

	//if (me!=nullptr && this->IsLocalPlayerController())// && this->IsLocalPlayerController()
	//{
	//	ServerOnPossess(me);
	//}
	//else
	//{
	//	OnPossess(me);
	//}
	//// playercontroller에 캐릭터 possess
	//if (this->GetPawn() == nullptr)
	//{
	//	

	//	me = GetWorld()->SpawnActor<AKartPlayer>(kartPlayer, param);
	//	//Possess(me);
	//	this->SetOwner(me);
	//}
	
	// playercontroller가 possess 하는 캐릭터
	//me = Cast<AKartPlayer>(this->GetPawn());
	////meOwner = Cast<AKartPlayer>(GetOwner());
	//
	//if (me != nullptr)
	//{
	//	// 플레이어 기본 속도 설정
	//	me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;	
	//}
	//
	// 플레이어 확인 디버그 메시지
	/*FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);*/

	//// 출발 카운트 사운드 재생
	//FTimerHandle DelayHandle;

	//GetWorldTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]() {
	//	if (startcountSound)
	//	{
	//		playingstartComp = UGameplayStatics::SpawnSound2D(GetWorld(), startcountSound);

	//		// 출발 카운트 사운드 유효성 검사
	//		if (playingstartComp)
	//		{
	//			playingstartComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
	//			playingstartComp->bAutoDestroy = true; // false : 재생 완료 후 자동으로 제거하지 않는다.

	//			playingstartComp->Play();
	//		}
	//	}
	//			}), 3.f, false);

	//network role 확인
	myLocalRole = GetLocalRole();
	myRemoteRole = GetRemoteRole();

}

// 정보를 화면에 출력하는 함수
void AMarioKartPlayerController::PrintLog()
{
	if (me != nullptr)
	{
		const FString PawnString = me != nullptr ? GetPawn()->GetName() : FString("No Pawn!");
		const FString localRoleString = UEnum::GetValueAsString<ENetRole>(myLocalRole);
		const FString remoteRoleString = UEnum::GetValueAsString<ENetRole>(myRemoteRole);
		const FString ownerString = me->GetOwner() != nullptr ? me->GetOwner()->GetName() : FString("No Owner!");
		const FString connectionString = GetNetConnection() != nullptr ? FString("Valid Connection") : FString("Invalid Connection");
		//AMarioKartPlayerController* pc = GetController<AMarioKartPlayerController>();
		const FString PlayerControllerString = this != nullptr ? this->GetName() : *FString("Invalid");
		const FString printString = FString::Printf(TEXT("PawnString : %s\nLocal Role: %s\nRemote Role: %s\nOwner Name: %s\nNet Connection : %s\nPlayerController : %s"), *PawnString, *localRoleString, *remoteRoleString, *ownerString, *connectionString, *PlayerControllerString);

		//const FString printString = FString::Printf(TEXT("Time : %.2f"), timeTest);
		DrawDebugString(GetWorld(), me->GetActorLocation(), printString, nullptr, FColor::White, 0, true);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, printString);
	}
	else
	{
		return;
	}
}

void AMarioKartPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	startcountTime += DeltaSeconds;

	//PrintLog();
	
	//timeTest += DeltaSeconds;
	
	// 글라이딩 조건
	if (bGlide && me->GetCharacterMovement()->IsFalling() && glideCount == 0)
	{
		Glide();
	}

	if (startcountTime >= 0.0f) // 출발 카운드 사운드 재생 후 주행 startcountTime >= 6.7f
	//if(RaceGM->Able2Play == true)
	{
		// 전진, 후진 주행 (가속, 속도)
		MultiMoveVertical();

		if (bisMovingback == true)
		{
			// 후진할 때 이동
			me->AddMovementInput(Direction(), currentSpeed);
		}
		else
		{
			// 드리프트 시작
			if (bisAcc == true && bisDrift == true)
			{
				// 드리프트 시간 누적
				driftTime += DeltaSeconds;

				// 드리프트 회전
				me->AddMovementInput(Direction(), currentSpeed);

				// 드리프트가 1초이상 지속됐을 때
				if (driftTime >= 1.2f && dashCount == 0)
				{
					//FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
					//FString DriftString = FString::Printf(TEXT("drift 3"));
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);

					// 대쉬 무조건 발동
					Dash(1.2f);

					// 드리프트 시간 리셋
					//driftTime = 0.0f;
				}
			}				
			else if(bisAcc == true && bisDrift == false)
			{
				// 드리프트 사운드 멈추기
				if (playingdriftComp)
				{
					playingdriftComp->Stop();
					playingdriftComp->SetActive(false);
					playingdriftComp = nullptr;

				}

				FVector Dir = Direction();

				if (bTestDebug)
					return;
				if (me != nullptr)
				{
					// 전진 주행 이동
					me->AddMovementInput(Direction(), currentSpeed);
					//UE_LOG(LogTemp, Warning, TEXT("currentSpeed : %.2f"), currentSpeed);
				}
			}
			else
			{
				if (me != nullptr)
				{
					// 가속키 누르지 않았을 때 서서히 감속
					me->AddMovementInput(Direction(), currentSpeed);
				}
			
			}
		}
	}
	else
	{
		return;
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
	InputComponent->BindAction(TEXT("Drift"), IE_Pressed, this, &AMarioKartPlayerController::Drift);
	InputComponent->BindAction(TEXT("Drift"), IE_Released, this, &AMarioKartPlayerController::Drift_released);

	// Direction 함수 디버그
	InputComponent->BindAction(TEXT("TestDebug"), IE_Pressed, this, &AMarioKartPlayerController::TestDebug);

	// 아이템 키 입력
	InputComponent->BindAction(TEXT("ItemUse"), IE_Pressed, this, &AMarioKartPlayerController::ItemUse);


}

void AMarioKartPlayerController::Acc()
{
	if (me == nullptr)
	{
		return;
	}

	bisAcc = true;

	// 주행 사운드 재생
	if (driveSound)
	{
		// soundbase 주행 사운드 사운드 오디오 컴포넌트 생성 및 초기화
		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

		// 주행 사운드 유효성 검사
		if (playingdriveComp)
		{
			playingdriveComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
			playingdriveComp->bAutoDestroy = true;

			// 주행 사운드 재생
			playingdriveComp->Play();

		}
	}

	
}

void AMarioKartPlayerController::Acc_released()
{
	if (me == nullptr)
	{
		return;
	}
	bisAcc = false;

	// 주행 사운드 멈추기
	if (playingdriveComp)
	{
		playingdriveComp->Stop();
		playingdriveComp->SetActive(false);
		playingdriveComp = nullptr;
	}


}

void AMarioKartPlayerController::MoveBack()
{
	if (me == nullptr)
	{
		return;
	}
	bisMovingback = true;

	// 주행 사운드 재생
	if (driveSound && bisDrift == false)
	{
		// soundbase 주행 사운드 사운드 오디오 컴포넌트 생성 및 초기화
		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

		// 주행 사운드 유효성 검사
		if (playingdriveComp)
		{
			playingdriveComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
			playingdriveComp->bAutoDestroy = true;

			// 주행 사운드 재생
			playingdriveComp->Play();

		}
	}

}

void AMarioKartPlayerController::MoveBack_released()
{
	if (me == nullptr)
	{
		return;
	}
	bisMovingback = false;

	// 주행 사운드 멈추기
	if (playingdriveComp)
	{
		playingdriveComp->Stop();
		playingdriveComp->SetActive(false);
		playingdriveComp = nullptr;
	}

}


void AMarioKartPlayerController::Drift()
{
	if (me == nullptr)
	{
		return;
	}
	bisDrift = true;

	//Startdriftjump();

	if (bisAcc == true && me->horizontalValue != 0.0f)
	{
		// 주행 사운드 멈추기
		if (playingdriveComp)
		{
			playingdriveComp->Stop();
			playingdriveComp->SetActive(false);
			playingdriveComp = nullptr;
		}

		if (driftSound)
		{
			// soundbase 사운드 오디오 컴포넌트 생성 및 초기화
			playingdriftComp = UGameplayStatics::SpawnSound2D(GetWorld(), driftSound);

			// 대쉬 사운드 & 플레이어 보이스 유효성 검사
			if (playingdriftComp)
			{
				playingdriftComp->SetSound(driftSound);
				playingdriftComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
				playingdriftComp->bAutoDestroy = true;

				// 사운드 재생
				playingdriftComp->Play();
			}
		}
	}
	else
	{
		//if (me)
		//{
		//	// 드리프트 기능
		//	//me->Jump();
		//}
		// 주행 사운드 멈추기
		if (playingdriveComp)
		{
			playingdriveComp->Stop();
			playingdriveComp->SetActive(false);
			playingdriveComp = nullptr;
		}
	}
}

void AMarioKartPlayerController::Drift_released()
{
	if (me == nullptr)
	{
		return;
	}
	bisDrift = false;

	// 대쉬 카운트 초기화
	dashCount = 0;

	// 대쉬 시간 초기화
	driftTime = 0.0f;

	// 드리프트 사운드 멈추기
	if (playingdriftComp)
	{
		playingdriftComp->Stop();
		playingdriftComp->SetActive(false);
		playingdriftComp = nullptr;

	}
 //	if (bisAcc == true)
	//{

	//	if (driveSound)
	//	{
	//		// soundbase 사운드 오디오 컴포넌트 생성 및 초기화
	//		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

	//		// 주행 사운드 & 플레이어 보이스 유효성 검사
	//		if (playingdriveComp)
	//		{
	//			playingdriveComp->SetSound(driveSound);
	//			playingdriveComp->bIsUISound = false; 
	//			playingdriveComp->bAutoDestroy = true;

	//			// 사운드 재생
 //				playingdriveComp->Play();
	//		}
	//	}
	//}
	//else
	//{
	//	driftTime = 0.0f;
	//}
	
}

 // 좌우 키를 눌렀을 때 실행
void AMarioKartPlayerController::Horizontal(float value)
{
	if (me == nullptr)
	{
		return;
	}

	// 좌우 입력값
	me->horizontalValue = value;

	// 좌우 입력 들어왔을 때(회전)
	if (me->horizontalValue && FMath::Abs(currentSpeed)>0.9f)
	{
		if (bisMovingback == true)
		{
			// 후진 회전
			hvalue = FMath::Lerp(me->horizontalValue, 0.0f, 0.4f);
			if (FMath::IsNearlyZero(currentSpeed))
			{
				return AddYawInput(0.0f);
			}
			else
			{
				AddYawInput(hvalue);
			}

		}
		else
		{
			if (bisAcc == true)
			{
				if (bisDrift == true)
				{
					// 드리프트(Drift) 좌우 회전
					if (me->horizontalValue >= 1.0f)
					{
						DriftBody(dashCount);
						//hvalue = 0.7f;
						hvalue = FMath::Lerp(0.5f, 0.6f, 0.02f);
						UE_LOG(LogTemp, Warning, TEXT("%.2f"), hvalue);
						if (FMath::IsNearlyZero(currentSpeed))
						{
							return AddYawInput(0.0f);
						}
						else
						{
							AddYawInput(hvalue);
						}

					}
					else
					{
						DriftBody(dashCount);
						//hvalue = -0.7f;
						hvalue = FMath::Lerp(-0.5f, -0.6f, -0.02f);
						UE_LOG(LogTemp, Warning, TEXT("%.2f"), hvalue);

						if (FMath::IsNearlyZero(currentSpeed))
						{
							return AddYawInput(0.0f);
						}
						else
						{
							AddYawInput(hvalue);
						}
					}					
						
				}
				else
				{	
					DriftBody(dashCount);

					// 전진 회전
					hvalue = FMath::Lerp(0.0f, me->horizontalValue, 0.4f);
						
					if (FMath::IsNearlyZero(currentSpeed))
					{
						return;
					}
					else
					{
						AddYawInput(hvalue);
					}
				}
			}
			else
			{
				if (me->horizontalValue >= 1.0f)
				{
					hvalue = FMath::Lerp(0.0f, me->horizontalValue, 0.4f);
					AddYawInput(hvalue);
				}
				else
				{
					hvalue = FMath::Lerp(me->horizontalValue, 0.0f, 0.4f);
					if (FMath::IsNearlyZero(currentSpeed))
					{
						return AddYawInput(0.0f);
					}
					else
					{
						AddYawInput(hvalue);
					}
				}
				
			}
		}
	} 
	else
	{
		AddYawInput(0.0f);
	}

}


void AMarioKartPlayerController::MultiMoveVertical()
{
	if (bisAcc == true || bisMovingback == true)
	{
		if (bisAcc == true)
		{
			// 전진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5f);

		}
		else
		{
			// 후진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * -1.5f);
		}
	}
	else
	{
		// 속도 0으로 조정
		currentSpeed = FMath::Lerp(currentSpeed, 0.0f, GetWorld()->GetDeltaSeconds() * 3.5f);
	}

}

// 이동 방향 벡터 반환 함수
FVector AMarioKartPlayerController::Direction()
{
	// 주행 방향 구하기
	FVector returnDirection = me->GetActorForwardVector();

	//if(driftTime)

	//UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());
	return returnDirection;
}

//// 드리프트 활성화
//void AMarioKartPlayerController::DashActivate(float dashTime)
//{
//	// 플레이어 보이스 재생
//	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);
//
//	// 드리프트 사운드 멈추기
//	if (playingdriftComp)
//	{
//		playingdriftComp->Stop();
//		playingdriftComp->SetActive(false);
//		playingdriftComp = nullptr;
//	}
//
//	// 주행 사운드 멈추기
//	if (playingdriveComp)
//	{
//		playingdriveComp->Stop();
//		playingdriveComp->SetActive(false);
//		playingdriveComp = nullptr;
//	}
//
//	// 대쉬 사운드 재생
//	if (dashSound)
//	{
//		// soundbase 대쉬 사운드 오디오 컴포넌트 생성 및 초기화
//		playingAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), dashSound);
//
//		// 대쉬 사운드 유효성 검사
//		if (playingAudioComp)
//		{
//			playingAudioComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
//			playingAudioComp->bAutoDestroy = false; // 재생 완료 후 자동으로 제거하지 않는다.
//
//			// 대쉬 사운드 재생
//			playingAudioComp->Play();
//		}
//	}
//
//
//	// 대쉬 카운트 증가
//	dashCount++;
//
//	// 플레이어 이동 속도 2000 으로 늘어남
//	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
//
//	// 드리프트 시간 동안 앞으로 빠르게 전진
//	me->AddMovementInput(Direction(), currentSpeed);
//	
//	// 대쉬 카메라 이펙트 설정
//	me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = true;
//	me->kartCamComp->PostProcessSettings.MotionBlurAmount = 1.0f;
//	me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = true;
//	me->kartCamComp->PostProcessSettings.MotionBlurMax = 50.0f;
//
//	// 대쉬 dashTime초 타이머(타이머 사용)
//	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
//		DriftBody(dashCount);
//
//		if (me->GetCharacterMovement() != nullptr)
//		{
//			me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
//		}
//
//		// 대쉬 사운드 멈추기대쉬 사운드
//		if (playingAudioComp)
//		{
// 			playingAudioComp->Stop();
//			playingAudioComp->SetActive(false);
//			playingAudioComp = nullptr;
//		}
//		 
//		if(bisAcc == true)
//		{
//			if (driveSound)
//			{
//				// soundbase 주행 사운드 오디오 컴포넌트 생성 및 초기화
//				playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);
//
//				// 주행 사운드 유효성 검사
//				if (playingdriveComp)
//				{
//					playingdriveComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
//					playingdriveComp->bAutoDestroy = false; // 재생 완료 후 자동으로 제거하지 않는다.
//
//					// 주행 사운드 재생
//					playingdriveComp->Play();
//				}
//			}
//		}
//
//		// 대쉬 카메라 이펙트 해제
//		me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = false;
//		me->kartCamComp->PostProcessSettings.MotionBlurAmount = 0.5f;
//		me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = false;
//		me->kartCamComp->PostProcessSettings.MotionBlurMax = 5.0f;
//		
//
//	}), dashTime, false);
//
//	//// 드리프트 해제
//	//driftTime = 0.0f;
//
//}

void AMarioKartPlayerController::Dash_Implementation(float dashTime)
{
	// 플레이어 보이스 재생
	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);

	// 드리프트 사운드 멈추기
	if (playingdriftComp)
	{
		playingdriftComp->Stop();
		playingdriftComp->SetActive(false);
		playingdriftComp = nullptr;
	}

	// 주행 사운드 멈추기
	if (playingdriveComp)
	{
		playingdriveComp->Stop();
		playingdriveComp->SetActive(false);
		playingdriveComp = nullptr;
	}

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


	// 대쉬 카운트 증가
	dashCount++;

	// 플레이어 이동 속도 2000 으로 늘어남
	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	// 드리프트 시간 동안 앞으로 빠르게 전진
	me->AddMovementInput(Direction(), currentSpeed);

	// 대쉬 카메라 이펙트 설정
	//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = true;
	//me->kartCamComp->PostProcessSettings.MotionBlurAmount = 1.0f;
	//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = true;
	//me->kartCamComp->PostProcessSettings.MotionBlurMax = 50.0f;

	// 대쉬 dashTime초 타이머(타이머 사용)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
		DriftBody(dashCount);

		if (me->GetCharacterMovement() != nullptr)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
		}

		// 대쉬 사운드 멈추기대쉬 사운드
		if (playingAudioComp)
		{
			playingAudioComp->Stop();
			playingAudioComp->SetActive(false);
			playingAudioComp = nullptr;
		}

		if (bisAcc == true)
		{
			if (driveSound)
			{
				// soundbase 주행 사운드 오디오 컴포넌트 생성 및 초기화
				playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

				// 주행 사운드 유효성 검사
				if (playingdriveComp)
				{
					playingdriveComp->bIsUISound = false; // 루프 걸었다면 ui 사운드로 설정하지 않는다.
					playingdriveComp->bAutoDestroy = false; // 재생 완료 후 자동으로 제거하지 않는다.

					// 주행 사운드 재생
					playingdriveComp->Play();
				}
			}
		}

		// 대쉬 카메라 이펙트 해제
		//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = false;
		//me->kartCamComp->PostProcessSettings.MotionBlurAmount = 0.5f;
		//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = false;
		//me->kartCamComp->PostProcessSettings.MotionBlurMax = 5.0f;


		}), dashTime, false);

	//// 드리프트 해제
	//driftTime = 0.0f;
}

// 드리프트 카트바디 회전 함수
void AMarioKartPlayerController::DriftBody(int32 DCount)
{
	if (DCount <= 0 && bisDrift == true)
	{
		if (me->horizontalValue >= 1.0f)
		{
			me->kartmeshComp->SetRelativeRotation(FRotator(0, 30, 0));
			me->kartwheelComp->SetRelativeRotation(FRotator(0, 30, 0));
			me->kartCharacterBody->SetRelativeRotation(FRotator(0, 30, 0));
		}
		else if(me->horizontalValue <= -1.0f)
		{
			me->kartmeshComp->SetRelativeRotation(FRotator(0, -30, 0));
			me->kartwheelComp->SetRelativeRotation(FRotator(0, -30, 0));
			me->kartCharacterBody->SetRelativeRotation(FRotator(0, -30, 0));

		}
		else
		{
			me->kartmeshComp->SetRelativeRotation(FRotator(0));
			me->kartwheelComp->SetRelativeRotation(FRotator(0));
			me->kartCharacterBody->SetRelativeRotation(FRotator(0));

		}
	}
	else
	{
		me->kartmeshComp->SetRelativeRotation(FRotator(0));
		me->kartwheelComp->SetRelativeRotation(FRotator(0));
		me->kartCharacterBody->SetRelativeRotation(FRotator(0));
	}
	
}	

// 드리프트 카트바디 원래 모습으로 회전 함수
void AMarioKartPlayerController::DriftBodyReturn()
{
	me->kartmeshComp->SetRelativeRotation(FRotator(0));
	me->kartwheelComp->SetRelativeRotation(FRotator(0));
	me->kartCharacterBody->SetRelativeRotation(FRotator(0));
}

void AMarioKartPlayerController::ItemUse()
{
	ItemActivate();
}

void AMarioKartPlayerController::ItemActivate()
{
	// 대쉬 속도
	me->GetCharacterMovement()->MaxWalkSpeed = 4000.0f;

	// 대쉬 1.8초 동안 지속
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([&]() {
		me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
		}), 1.8f, false);

}


void AMarioKartPlayerController::TestDebug()
{
	bTestDebug = !bTestDebug;
}

void AMarioKartPlayerController::Glide()
{
	velocityZ = me->GetCharacterMovement()->Velocity.Z;

	/*UE_LOG(LogTemp, Warning, TEXT("%.2f, %.2f, %.2f"), me->GetCharacterMovement()->Velocity.X, me->GetCharacterMovement()->Velocity.Y, me->GetCharacterMovement()->Velocity.Z);
	UE_LOG(LogTemp, Warning, TEXT("GlideOn"));*/

	// 낙하산 kartParachute
	me->kartParachute->SetVisibility(true);


	// 공중에서 천천히 내려오게 한다.
	me->GetCharacterMovement()->GravityScale = 0.2f;
	me->GetCharacterMovement()->AirControl = 1.0f;
	me->GetCharacterMovement()->Velocity.X += 800;
	me->GetCharacterMovement()->Velocity.Y += 800;
	me->GetCharacterMovement()->Velocity.Z += 300;
	//UE_LOG(LogTemp, Warning, TEXT("%.2f, %.2f, %.2f"), me->GetCharacterMovement()->Velocity.X, me->GetCharacterMovement()->Velocity.Y, me->GetCharacterMovement()->Velocity.Z);

	GetWorldTimerManager().SetTimer(glideDelay, FTimerDelegate::CreateLambda([&]() {
		me->GetCharacterMovement()->GravityScale = 1.0f;
		me->GetCharacterMovement()->AirControl = 0.05f;
		me->kartParachute->SetVisibility(false);
		glideCount = 0;
		/*UE_LOG(LogTemp, Warning, TEXT("%.2f, %.2f, %.2f"), me->GetCharacterMovement()->Velocity.X, me->GetCharacterMovement()->Velocity.Y, me->GetCharacterMovement()->Velocity.Z);
		UE_LOG(LogTemp, Warning, TEXT("GlideOff"));*/

		}), 4.5f, false);

	glideCount++;
	
}

void AMarioKartPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Warning, TEXT("OnPossess"));

	me = Cast<AKartPlayer>(aPawn);

	if (HasAuthority())
	{
		ServerOnPossess(aPawn);
	}

	//if (this)
	//{
	//	me = Cast<AKartPlayer>(this->GetPawn());
	//	//meOwner = Cast<AKartPlayer>(GetOwner());
	//	//UE_LOG(LogTemp, Warning, TEXT("%d"), me);
	//	FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);
	//	//UE_LOG(LogTemp, Warning, TEXT("%d"), me->GetOwner());

	//	if (me != nullptr)
	//	{
	//		// 플레이어 기본 속도 설정
	//		me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
	//	}
	//}
}

void AMarioKartPlayerController::ServerOnPossess_Implementation(APawn* aPawn)
{
	//OnPossess(aPawn);
	me = Cast<AKartPlayer>(aPawn);

	//MulticastOnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("ServerOnPossess"));

}

//void AMarioKartPlayerController::MulticastOnPossess_Implementation(APawn* aPawn)
//{
//	OnPossess(aPawn);
//}

//void AMarioKartPlayerController::ServerPossessPawn_Implementation()
//{
//	if (gm != nullptr)
//	{
//		APawn* playerpawn = GetPawn();
//
//	}
//}

void AMarioKartPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMarioKartPlayerController, timeTest);
	DOREPLIFETIME(AMarioKartPlayerController, currentSpeed);
	DOREPLIFETIME(AMarioKartPlayerController, me);
}