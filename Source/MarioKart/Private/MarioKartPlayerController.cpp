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


AMarioKartPlayerController::AMarioKartPlayerController()
{

	PrimaryActorTick.bCanEverTick = true;

	// �⺻������ PlayerID�� 0���� ����
	PlayerID = 0;

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
}

void AMarioKartPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ���� �ҷ�����
	me = Cast<AKartPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// playercontroller�� possess�� ĳ���� �ҷ�����
	//AMario
	
	meOwner = Cast<AKartPlayer>(GetOwner());
	
	if (me != nullptr)
	{
		// �÷��̾� �⺻ �ӵ� ����
		me->GetCharacterMovement()->MaxWalkSpeed = 1300.0f;	
	}
	
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

	if (driftjumpCurve != nullptr)
	{

		// Ÿ�Ӷ��� �ʱ�ȭ
		/*UTimelineComponent* timelineComp = NewObject<UTimelineComponent>(this, FName("driftjumpTimeline"));
		timelineComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		timelineComp->SetNetAddressable();
		timelineComp->SetPropertySetObject(this);
		timelineComp->RegisterComponent();*/
	
		
		// �帮��Ʈ ���� �پ� ���� Ŀ�� �
		// Ÿ�Ӷ��� ���
		FOnTimelineFloat onProgressdriftjump;
		onProgressdriftjump.BindUFunction(this, FName("Ondriftjump"));

		driftjumpTimeline.AddInterpFloat(driftjumpCurve, onProgressdriftjump);
		driftjumpTimeline.SetTimelineLength(0.5f);
	}

}

void AMarioKartPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	startcountTime += DeltaSeconds;
	
	//driftjumpTimeline.TickTimeline(DeltaSeconds);

	if (startcountTime >= 6.7f) // ��� ī��� ���� ��� �� ���� startcountTime >= 6.7f
	{
		// ����, ���� ���� (����, �ӵ�)
		MoveVertical();

		if (bisMovingback == true)
		{
			// ������ �� �̵�
			me->AddMovementInput(Direction(), currentSpeed);
		}
		else
		{
			if (bisAcc == true)
			{
				if (bisJump == true)
				{
					// �帮��Ʈ ����
					// �帮��Ʈ �ð� ����
					driftTime += DeltaSeconds;
					me->AddMovementInput(Direction(), currentSpeed);

					// �帮��Ʈ�� 3���̻� ���ӵ��� ��
					if (driftTime >= 3.0f)
					{
						//FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
						//FString DriftString = FString::Printf(TEXT("drift 3"));
						//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
						// 3�� �帮��Ʈ ������ �ߵ�
						DriftActivate(3.0f);

						// �帮��Ʈ �ð� ����
						driftTime = 0.0f;
					}
				}
				else
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
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMarioKartPlayerController::Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMarioKartPlayerController::Jump_released);

	// Direction �Լ� �����
	InputComponent->BindAction(TEXT("TestDebug"), IE_Pressed, this, &AMarioKartPlayerController::TestDebug);

	// ������ Ű �Է�
	InputComponent->BindAction(TEXT("ItemUse"), IE_Pressed, this, &AMarioKartPlayerController::ItemUse);


}

void AMarioKartPlayerController::Acc()
{
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

	UE_LOG(LogTemp, Warning, TEXT("Acc: %d"), bisAcc ? true : false);
}

void AMarioKartPlayerController::Acc_released()
{
	bisAcc = false;

	// ���� ���� ���߱�
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
	bisMovingback = true;

	// ���� ���� ���
	if (driveSound && bisJump == false)
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

	UE_LOG(LogTemp, Warning, TEXT("MoveBack: %d"), bisMovingback ? true : false);
}

void AMarioKartPlayerController::MoveBack_released()
{
	bisMovingback = false;

	// ���� ���� ���߱�
	if (playingdriveComp)
	{
		playingdriveComp->Stop();
		playingdriveComp->SetActive(false);
		playingdriveComp = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("MoveBack_released: %d"), bisMovingback ? true : false);
}


void AMarioKartPlayerController::Jump()
{
	bisJump = true;

	//Startdriftjump();

	if (bisAcc == true && horizontalValue != 0.0f)
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
		if (me)
		{
			// �帮��Ʈ ���
			me->Jump();
		}
		// ���� ���� ���߱�
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
	bisJump = false;	

	// �帮��Ʈ ���� ���߱�
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
			// soundbase ���� ����� ������Ʈ ���� �� �ʱ�ȭ
			playingdriveComp = UGameplayStatics::SpawnSound2D(GetWorld(), driveSound);

			// ���� ���� & �÷��̾� ���̽� ��ȿ�� �˻�
			if (playingdriveComp)
			{
				playingdriveComp->SetSound(driveSound);
				playingdriveComp->bIsUISound = false; 
				playingdriveComp->bAutoDestroy = true;

				// ���� ���
 				playingdriveComp->Play();
			}
		}
	}	

	// �帮��Ʈ �ߵ�
	if (driftTime >= 2.0f)
	{
		FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
		FString DriftString = FString::Printf(TEXT("drift 2"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
		// �帮��Ʈ�� 2���̻� ���ӵ��� ��
		// 2�� �帮��Ʈ
		DriftActivate(2.0f);

		// �帮��Ʈ ����
		driftTime = 0.0f;

	}
	else if (driftTime >= 1.0f)
	{
		FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
		FString DriftString = FString::Printf(TEXT("drift 1"));
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
		// 1�� �帮��Ʈ
		DriftActivate(1.0f);

		// �帮��Ʈ ����
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
	// �¿� �Է°�
	horizontalValue = value;

	// �¿� �Է� ������ ��(ȸ��)
	if (FMath::Abs(horizontalValue) != 0.0f)
	{
		if (bisMovingback == true || currentSpeed != 0)
		{
			// ���� ȸ��
			float movigbackValue = FMath::Lerp(horizontalValue, 0.0f, 0.3f);
			AddYawInput(movigbackValue);
			UE_LOG(LogTemp, Warning, TEXT("movigbackValue : %.2f"), movigbackValue);

		}
		else
		{
			if (bisAcc == true || currentSpeed != 0)
			{
				if (bisJump == true)
				{
					float driftValue = FMath::Lerp(0.0f, horizontalValue, 0.3f); // ȸ����
					AddYawInput(driftValue);
				}
				else
				{
					// ���� ȸ��
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
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5);


		}
		else
		{
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed * -1.8f, GetWorld()->GetDeltaSeconds());
		}
	}
	else
	{
		// �ӵ� 0���� ����
		currentSpeed = FMath::Lerp(currentSpeed, 0.0f, GetWorld()->GetDeltaSeconds() * 3.0f);

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

void AMarioKartPlayerController::DriftActivate(float dashActiveTime)
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

	// �÷��̾� �̵� �ӵ� 2000 ���� �þ
	me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

	// �帮��Ʈ �ð� ���� ������ ������ ����
	me->AddMovementInput(Direction(), currentSpeed);

	// �帮��Ʈ ���� dashActiveTime��(Ÿ�̸� ���)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
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

		if(bisAcc == true)
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

					// �뽬 ���� ���
					playingdriveComp->Play();
				}
			}
		}

		}), dashActiveTime, false);

	// �帮��Ʈ ����
	driftTime = 0.0f;

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
