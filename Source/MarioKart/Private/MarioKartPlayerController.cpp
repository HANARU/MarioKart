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

	if (driftjumpCurve != nullptr)
	{

		// 타임라인 초기화
		/*UTimelineComponent* timelineComp = NewObject<UTimelineComponent>(this, FName("driftjumpTimeline"));
		timelineComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		timelineComp->SetNetAddressable();
		timelineComp->SetPropertySetObject(this);
		timelineComp->RegisterComponent();*/
	
		
		// 드리프트 동안 뛰어 오를 커브 곡선
		// 타임라인 등록
		FOnTimelineFloat onProgressdriftjump;
		onProgressdriftjump.BindUFunction(this, FName("Ondriftjump"));

		driftjumpTimeline.AddInterpFloat(driftjumpCurve, onProgressdriftjump);
		driftjumpTimeline.SetTimelineLength(0.5f);
	}

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

	PrintLog();
	
	//timeTest += DeltaSeconds;
	//driftjumpTimeline.TickTimeline(DeltaSeconds);

	if (startcountTime >= 6.7f) // 출발 카운드 사운드 재생 후 주행 startcountTime >= 6.7f
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
			if (bisAcc == true)
			{
				if (bisJump == true)
				{
					// 드리프트 시작
					// 드리프트 시간 누적
					driftTime += DeltaSeconds;
					me->AddMovementInput(Direction(), currentSpeed);

					// 드리프트가 3초이상 지속됐을 때
					if (driftTime >= 3.0f)
					{
						//FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
						//FString DriftString = FString::Printf(TEXT("drift 3"));
						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
						// 3단 드리프트 무조건 발동
						DriftActivate(3.0f);

						// 드리프트 시간 리셋
						driftTime = 0.0f;
					}
				}
				else
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
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMarioKartPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMarioKartPlayerController::Jump_released);

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

	UE_LOG(LogTemp, Warning, TEXT("Acc: %d"), bisAcc ? true : false);
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

	UE_LOG(LogTemp, Warning, TEXT("Acc_released: %d"), bisAcc ? true : false);

}

void AMarioKartPlayerController::MoveBack()
{
	if (me == nullptr)
	{
		return;
	}
	bisMovingback = true;

	// 주행 사운드 재생
	if (driveSound && bisJump == false)
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


void AMarioKartPlayerController::Jump()
{
	if (me == nullptr)
	{
		return;
	}
	bisJump = true;

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
		if (me)
		{
			// 드리프트 기능
			me->Jump();
		}
		// 주행 사운드 멈추기
		if (playingdriveComp)
		{
			playingdriveComp->Stop();
			playingdriveComp->SetActive(false);
			playingdriveComp = nullptr;
		}
	}
}

void AMarioKartPlayerController::Jump_released()
{
	if (me == nullptr)
	{
		return;
	}
	bisJump = false;	

	// 드리프트 사운드 멈추기
	if (playingdriftComp)
	{
		playingdriftComp->Stop();
		playingdriftComp->SetActive(false);
		playingdriftComp = nullptr;

	}

 	if (bisAcc == true)
	{

		if (driveSound)
		{
			// soundbase 사운드 오디오 컴포넌트 생성 및 초기화
			playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

			// 주행 사운드 & 플레이어 보이스 유효성 검사
			if (playingdriveComp)
			{
				playingdriveComp->SetSound(driveSound);
				playingdriveComp->bIsUISound = false; 
				playingdriveComp->bAutoDestroy = true;

				// 사운드 재생
 				playingdriveComp->Play();
			}
		}
	}	

	// 드리프트 발동
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

	if (value)
	{
		// 좌우 입력 들어왔을 때(회전)
		if (FMath::Abs(me->horizontalValue) != 0.0f)
		{
			if (bisMovingback == true)
			{
				// 후진 회전
				float movigbackValue = FMath::Lerp(me->horizontalValue, 0.0f, 0.9f);
				if (FMath::IsNearlyZero(currentSpeed))
				{
					return AddYawInput(0.0f);
				}
				else
				{
					AddYawInput(movigbackValue);
				}
				
				UE_LOG(LogTemp, Warning, TEXT("movigbackValue : %.2f"), movigbackValue);
				UE_LOG(LogTemp, Warning, TEXT("%d"), FMath::IsNearlyZero(currentSpeed));

			}
			else
			{
				if (bisAcc == true)
				{
					if (bisJump == true)
					{
						//float driftValue = FMath::Lerp(0.0f, me->horizontalValue, 0.3f); // 회전율
						if (me->horizontalValue >= 1.0f)
						{
							float driftValue = 0.3f;
							if (FMath::IsNearlyZero(currentSpeed))
							{
								return AddYawInput(0.0f);
							}
							else
							{
								AddYawInput(driftValue);
							}
							UE_LOG(LogTemp, Warning, TEXT("driftValue : %.2f"), driftValue);
							UE_LOG(LogTemp, Warning, TEXT("%d"), FMath::IsNearlyZero(currentSpeed));
						}
						else
						{
							float driftValue = -0.3f;
							if (FMath::IsNearlyZero(currentSpeed))
							{
								return AddYawInput(0.0f);
							}
							else
							{
								AddYawInput(driftValue);
							}
							UE_LOG(LogTemp, Warning, TEXT("driftValue : %.2f"), driftValue);
							UE_LOG(LogTemp, Warning, TEXT("%d"), FMath::IsNearlyZero(currentSpeed));
						}					
						
					}
					else
					{
						// 전진 회전
						float accValue = FMath::Lerp(0.0f, me->horizontalValue, 0.1f);
						
						if (FMath::IsNearlyZero(currentSpeed))
						{
							return;
						}
						else
						{
							AddYawInput(accValue);
						}
						UE_LOG(LogTemp, Warning, TEXT("accValue : %.2f"), accValue);
						UE_LOG(LogTemp, Warning, TEXT("%d"), FMath::IsNearlyZero(currentSpeed));

					}
				}
			}
		}
	}
 

}


void AMarioKartPlayerController::MultiMoveVertical()
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
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * -1.5f);
		}
	}
	else
	{
		// 속도 0으로 조정
		currentSpeed = FMath::Lerp(currentSpeed, 0.0f, GetWorld()->GetDeltaSeconds() * 3.5f);
		if (FMath::IsNearlyZero(currentSpeed))
		{
			currentSpeed = 0.0f;
		}

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

void AMarioKartPlayerController::DriftActivate(float dashActiveTime)
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

	// 플레이어 이동 속도 2000 으로 늘어남
	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	// 드리프트 시간 동안 앞으로 빠르게 전진
	me->AddMovementInput(Direction(), currentSpeed);

	// 드리프트 지속 dashActiveTime초(타이머 사용)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
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

		if(bisAcc == true)
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

					// 대쉬 사운드 재생
					playingdriveComp->Play();
				}
			}
		}

		}), dashActiveTime, false);

	// 드리프트 해제
	driftTime = 0.0f;

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

void AMarioKartPlayerController::Ondriftjump(float Output)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, FString::Printf(TEXT("%.3f"), Output));
	//UE_LOG(LogTemp, Warning, TEXT("%.3f"), Output);

	//me->GetCharacterMovement()->Velocity = currentDir * driftjumpPower * Output;
}

void AMarioKartPlayerController::Startdriftjump()
{
	driftjumpTimeline.PlayFromStart();
	/*if (me->GetCharacterMovement()->Velocity.IsNearlyZero())
	{
		currentDir = me->GetActorUpVector();
	}
	else
	{
		currentDir = me->GetCharacterMovement()->Velocity;
	}*/
	//driftjumpTimeline.SetLooping(false);
 	
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
	DOREPLIFETIME(AMarioKartPlayerController, me);
}