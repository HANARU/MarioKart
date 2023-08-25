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
#include "Net/UnrealNetwork.h" // �𸮾� ��Ʈ��ũ ���
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "KartPlayerAnimInstance.h"
#include "GM_Race.h"




AMarioKartPlayerController::AMarioKartPlayerController()
{

	PrimaryActorTick.bCanEverTick = true;

	// �⺻������ PlayerID�� 0���� ����
	//PlayerID = 0;

	

	// ���� ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempdriveSound(TEXT("SoundWave'/Game/5_FX/Audio/play_drive_kart.play_drive_kart'"));

	if (TempdriveSound.Succeeded())
	{
		driveSound = TempdriveSound.Object;
	}

	// �뽬 ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_short.play_dash_short'"));

	if (TempdashSound.Succeeded())
	{
		dashSound = TempdashSound.Object;
	}

	// �帮��Ʈ ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempdriftSound(TEXT("SoundWave'/Game/5_FX/Audio/play_DRIFT_STEER.play_DRIFT_STEER'"));

	if (TempdriftSound.Succeeded())
	{
		driftSound = TempdriftSound.Object;
	}

	// ������ ���̽� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempmariovoiceSound(TEXT("SoundWave'/Game/5_FX/Audio/play_mario-voice_.play_mario-voice_'"));

	if (TempmariovoiceSound.Succeeded())
	{
		mariovoiceSound = TempmariovoiceSound.Object;
	}
	
	// ��� ī��� ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempstartcountSound(TEXT("SoundWave'/Game/5_FX/Audio/play_Start-Sound-Effect_.play_Start-Sound-Effect_'"));

	if (TempstartcountSound.Succeeded())
	{
		startcountSound = TempstartcountSound.Object;
	}

	// Ÿ�Ӷ���
	//driftjumpCurve = nullptr;

	// ���� ���� ��� ���
	bReplicates = true;
	SetReplicateMovement(true);
}

void AMarioKartPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���Ӹ��
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
	//// playercontroller�� ĳ���� possess
	//if (this->GetPawn() == nullptr)
	//{
	//	

	//	me = GetWorld()->SpawnActor<AKartPlayer>(kartPlayer, param);
	//	//Possess(me);
	//	this->SetOwner(me);
	//}
	
	// playercontroller�� possess �ϴ� ĳ����
	//me = Cast<AKartPlayer>(this->GetPawn());
	////meOwner = Cast<AKartPlayer>(GetOwner());
	//
	//if (me != nullptr)
	//{
	//	// �÷��̾� �⺻ �ӵ� ����
	//	me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;	
	//}
	//
	// �÷��̾� Ȯ�� ����� �޽���
	/*FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);*/

	//// ��� ī��Ʈ ���� ���
	//FTimerHandle DelayHandle;

	//GetWorldTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this]() {
	//	if (startcountSound)
	//	{
	//		playingstartComp = UGameplayStatics::SpawnSound2D(GetWorld(), startcountSound);

	//		// ��� ī��Ʈ ���� ��ȿ�� �˻�
	//		if (playingstartComp)
	//		{
	//			playingstartComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
	//			playingstartComp->bAutoDestroy = true; // false : ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.

	//			playingstartComp->Play();
	//		}
	//	}
	//			}), 3.f, false);

	//network role Ȯ��
	myLocalRole = GetLocalRole();
	myRemoteRole = GetRemoteRole();

}

// ������ ȭ�鿡 ����ϴ� �Լ�
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
	
	// �۶��̵� ����
	if (bGlide && me->GetCharacterMovement()->IsFalling() && glideCount == 0)
	{
		Glide();
	}

	if (startcountTime >= 0.0f) // ��� ī��� ���� ��� �� ���� startcountTime >= 6.7f
	//if(RaceGM->Able2Play == true)
	{
		// ����, ���� ���� (����, �ӵ�)
		MultiMoveVertical();

		if (bisMovingback == true)
		{
			// ������ �� �̵�
			me->AddMovementInput(Direction(), currentSpeed);
		}
		else
		{
			// �帮��Ʈ ����
			if (bisAcc == true && bisDrift == true)
			{
				// �帮��Ʈ �ð� ����
				driftTime += DeltaSeconds;

				// �帮��Ʈ ȸ��
				me->AddMovementInput(Direction(), currentSpeed);

				// �帮��Ʈ�� 1���̻� ���ӵ��� ��
				if (driftTime >= 1.2f && dashCount == 0)
				{
					//FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
					//FString DriftString = FString::Printf(TEXT("drift 3"));
					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);

					// �뽬 ������ �ߵ�
					Dash(1.2f);

					// �帮��Ʈ �ð� ����
					//driftTime = 0.0f;
				}
			}				
			else if(bisAcc == true && bisDrift == false)
			{
				// �帮��Ʈ ���� ���߱�
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
					// ���� ���� �̵�
					me->AddMovementInput(Direction(), currentSpeed);
					//UE_LOG(LogTemp, Warning, TEXT("currentSpeed : %.2f"), currentSpeed);
				}
			}
			else
			{
				if (me != nullptr)
				{
					// ����Ű ������ �ʾ��� �� ������ ����
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
	//�Է� �Լ� ���ε�
	Super::SetupInputComponent();

	// �¿� �Է�
	InputComponent->BindAxis(TEXT("Horizontal"), this, &AMarioKartPlayerController::Horizontal);

	// ����Ű �Է�
	InputComponent->BindAction(TEXT("Acc"), IE_Pressed, this, &AMarioKartPlayerController::Acc);
	InputComponent->BindAction(TEXT("Acc"), IE_Released, this, &AMarioKartPlayerController::Acc_released);

	// ����Ű �Է�
	InputComponent->BindAction(TEXT("MoveBack"), IE_Pressed, this, &AMarioKartPlayerController::MoveBack);
	InputComponent->BindAction(TEXT("MoveBack"), IE_Released, this, &AMarioKartPlayerController::MoveBack_released);

	// �帮��Ʈ (����Ű �Է�)
	InputComponent->BindAction(TEXT("Drift"), IE_Pressed, this, &AMarioKartPlayerController::Drift);
	InputComponent->BindAction(TEXT("Drift"), IE_Released, this, &AMarioKartPlayerController::Drift_released);

	// Direction �Լ� �����
	InputComponent->BindAction(TEXT("TestDebug"), IE_Pressed, this, &AMarioKartPlayerController::TestDebug);

	// ������ Ű �Է�
	InputComponent->BindAction(TEXT("ItemUse"), IE_Pressed, this, &AMarioKartPlayerController::ItemUse);


}

void AMarioKartPlayerController::Acc()
{
	if (me == nullptr)
	{
		return;
	}

	bisAcc = true;

	// ���� ���� ���
	if (driveSound)
	{
		// soundbase ���� ���� ���� ����� ������Ʈ ���� �� �ʱ�ȭ
		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

		// ���� ���� ��ȿ�� �˻�
		if (playingdriveComp)
		{
			playingdriveComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
			playingdriveComp->bAutoDestroy = true;

			// ���� ���� ���
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

	// ���� ���� ���߱�
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

	// ���� ���� ���
	if (driveSound && bisDrift == false)
	{
		// soundbase ���� ���� ���� ����� ������Ʈ ���� �� �ʱ�ȭ
		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

		// ���� ���� ��ȿ�� �˻�
		if (playingdriveComp)
		{
			playingdriveComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
			playingdriveComp->bAutoDestroy = true;

			// ���� ���� ���
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

	// ���� ���� ���߱�
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
		// ���� ���� ���߱�
		if (playingdriveComp)
		{
			playingdriveComp->Stop();
			playingdriveComp->SetActive(false);
			playingdriveComp = nullptr;
		}

		if (driftSound)
		{
			// soundbase ���� ����� ������Ʈ ���� �� �ʱ�ȭ
			playingdriftComp = UGameplayStatics::SpawnSound2D(GetWorld(), driftSound);

			// �뽬 ���� & �÷��̾� ���̽� ��ȿ�� �˻�
			if (playingdriftComp)
			{
				playingdriftComp->SetSound(driftSound);
				playingdriftComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
				playingdriftComp->bAutoDestroy = true;

				// ���� ���
				playingdriftComp->Play();
			}
		}
	}
	else
	{
		//if (me)
		//{
		//	// �帮��Ʈ ���
		//	//me->Jump();
		//}
		// ���� ���� ���߱�
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

	// �뽬 ī��Ʈ �ʱ�ȭ
	dashCount = 0;

	// �뽬 �ð� �ʱ�ȭ
	driftTime = 0.0f;

	// �帮��Ʈ ���� ���߱�
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
	//		// soundbase ���� ����� ������Ʈ ���� �� �ʱ�ȭ
	//		playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

	//		// ���� ���� & �÷��̾� ���̽� ��ȿ�� �˻�
	//		if (playingdriveComp)
	//		{
	//			playingdriveComp->SetSound(driveSound);
	//			playingdriveComp->bIsUISound = false; 
	//			playingdriveComp->bAutoDestroy = true;

	//			// ���� ���
 //				playingdriveComp->Play();
	//		}
	//	}
	//}
	//else
	//{
	//	driftTime = 0.0f;
	//}
	
}

 // �¿� Ű�� ������ �� ����
void AMarioKartPlayerController::Horizontal(float value)
{
	if (me == nullptr)
	{
		return;
	}

	// �¿� �Է°�
	me->horizontalValue = value;

	// �¿� �Է� ������ ��(ȸ��)
	if (me->horizontalValue && FMath::Abs(currentSpeed)>0.9f)
	{
		if (bisMovingback == true)
		{
			// ���� ȸ��
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
					// �帮��Ʈ(Drift) �¿� ȸ��
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

					// ���� ȸ��
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
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5f);

		}
		else
		{
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * -1.5f);
		}
	}
	else
	{
		// �ӵ� 0���� ����
		currentSpeed = FMath::Lerp(currentSpeed, 0.0f, GetWorld()->GetDeltaSeconds() * 3.5f);
	}

}

// �̵� ���� ���� ��ȯ �Լ�
FVector AMarioKartPlayerController::Direction()
{
	// ���� ���� ���ϱ�
	FVector returnDirection = me->GetActorForwardVector();

	//if(driftTime)

	//UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());
	return returnDirection;
}

//// �帮��Ʈ Ȱ��ȭ
//void AMarioKartPlayerController::DashActivate(float dashTime)
//{
//	// �÷��̾� ���̽� ���
//	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);
//
//	// �帮��Ʈ ���� ���߱�
//	if (playingdriftComp)
//	{
//		playingdriftComp->Stop();
//		playingdriftComp->SetActive(false);
//		playingdriftComp = nullptr;
//	}
//
//	// ���� ���� ���߱�
//	if (playingdriveComp)
//	{
//		playingdriveComp->Stop();
//		playingdriveComp->SetActive(false);
//		playingdriveComp = nullptr;
//	}
//
//	// �뽬 ���� ���
//	if (dashSound)
//	{
//		// soundbase �뽬 ���� ����� ������Ʈ ���� �� �ʱ�ȭ
//		playingAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), dashSound);
//
//		// �뽬 ���� ��ȿ�� �˻�
//		if (playingAudioComp)
//		{
//			playingAudioComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
//			playingAudioComp->bAutoDestroy = false; // ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.
//
//			// �뽬 ���� ���
//			playingAudioComp->Play();
//		}
//	}
//
//
//	// �뽬 ī��Ʈ ����
//	dashCount++;
//
//	// �÷��̾� �̵� �ӵ� 2000 ���� �þ
//	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
//
//	// �帮��Ʈ �ð� ���� ������ ������ ����
//	me->AddMovementInput(Direction(), currentSpeed);
//	
//	// �뽬 ī�޶� ����Ʈ ����
//	me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = true;
//	me->kartCamComp->PostProcessSettings.MotionBlurAmount = 1.0f;
//	me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = true;
//	me->kartCamComp->PostProcessSettings.MotionBlurMax = 50.0f;
//
//	// �뽬 dashTime�� Ÿ�̸�(Ÿ�̸� ���)
//	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
//		DriftBody(dashCount);
//
//		if (me->GetCharacterMovement() != nullptr)
//		{
//			me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
//		}
//
//		// �뽬 ���� ���߱�뽬 ����
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
//				// soundbase ���� ���� ����� ������Ʈ ���� �� �ʱ�ȭ
//				playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);
//
//				// ���� ���� ��ȿ�� �˻�
//				if (playingdriveComp)
//				{
//					playingdriveComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
//					playingdriveComp->bAutoDestroy = false; // ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.
//
//					// ���� ���� ���
//					playingdriveComp->Play();
//				}
//			}
//		}
//
//		// �뽬 ī�޶� ����Ʈ ����
//		me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = false;
//		me->kartCamComp->PostProcessSettings.MotionBlurAmount = 0.5f;
//		me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = false;
//		me->kartCamComp->PostProcessSettings.MotionBlurMax = 5.0f;
//		
//
//	}), dashTime, false);
//
//	//// �帮��Ʈ ����
//	//driftTime = 0.0f;
//
//}

void AMarioKartPlayerController::Dash_Implementation(float dashTime)
{
	// �÷��̾� ���̽� ���
	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);

	// �帮��Ʈ ���� ���߱�
	if (playingdriftComp)
	{
		playingdriftComp->Stop();
		playingdriftComp->SetActive(false);
		playingdriftComp = nullptr;
	}

	// ���� ���� ���߱�
	if (playingdriveComp)
	{
		playingdriveComp->Stop();
		playingdriveComp->SetActive(false);
		playingdriveComp = nullptr;
	}

	// �뽬 ���� ���
	if (dashSound)
	{
		// soundbase �뽬 ���� ����� ������Ʈ ���� �� �ʱ�ȭ
		playingAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), dashSound);

		// �뽬 ���� ��ȿ�� �˻�
		if (playingAudioComp)
		{
			playingAudioComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
			playingAudioComp->bAutoDestroy = false; // ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.

			// �뽬 ���� ���
			playingAudioComp->Play();
		}
	}


	// �뽬 ī��Ʈ ����
	dashCount++;

	// �÷��̾� �̵� �ӵ� 2000 ���� �þ
	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	// �帮��Ʈ �ð� ���� ������ ������ ����
	me->AddMovementInput(Direction(), currentSpeed);

	// �뽬 ī�޶� ����Ʈ ����
	//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = true;
	//me->kartCamComp->PostProcessSettings.MotionBlurAmount = 1.0f;
	//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = true;
	//me->kartCamComp->PostProcessSettings.MotionBlurMax = 50.0f;

	// �뽬 dashTime�� Ÿ�̸�(Ÿ�̸� ���)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
		DriftBody(dashCount);

		if (me->GetCharacterMovement() != nullptr)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;
		}

		// �뽬 ���� ���߱�뽬 ����
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
				// soundbase ���� ���� ����� ������Ʈ ���� �� �ʱ�ȭ
				playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

				// ���� ���� ��ȿ�� �˻�
				if (playingdriveComp)
				{
					playingdriveComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
					playingdriveComp->bAutoDestroy = false; // ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.

					// ���� ���� ���
					playingdriveComp->Play();
				}
			}
		}

		// �뽬 ī�޶� ����Ʈ ����
		//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurAmount = false;
		//me->kartCamComp->PostProcessSettings.MotionBlurAmount = 0.5f;
		//me->kartCamComp->PostProcessSettings.bOverride_MotionBlurMax = false;
		//me->kartCamComp->PostProcessSettings.MotionBlurMax = 5.0f;


		}), dashTime, false);

	//// �帮��Ʈ ����
	//driftTime = 0.0f;
}

// �帮��Ʈ īƮ�ٵ� ȸ�� �Լ�
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

// �帮��Ʈ īƮ�ٵ� ���� ������� ȸ�� �Լ�
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
	// �뽬 �ӵ�
	me->GetCharacterMovement()->MaxWalkSpeed = 4000.0f;

	// �뽬 1.8�� ���� ����
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

	// ���ϻ� kartParachute
	me->kartParachute->SetVisibility(true);


	// ���߿��� õõ�� �������� �Ѵ�.
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
	//		// �÷��̾� �⺻ �ӵ� ����
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