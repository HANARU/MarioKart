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

	// �뽬 ���� ��������
	ConstructorHelpers::FObjectFinder<USoundBase> TempdashSound(TEXT("SoundWave'/Game/5_FX/Audio/play_dash_.play_dash_'"));

	if (TempdashSound.Succeeded())
	{
		dashSound = TempdashSound.Object;
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
}

void AMarioKartPlayerController::BeginPlay()
{
	// �÷��̾� ĳ���� �ҷ�����
	me = Cast<AKartPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// �÷��̾� �⺻ �ӵ� ����
	me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	// �÷��̾� Ȯ�� ����� �޽���
	FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);

	// ��� ī��Ʈ ���� ���
	if (startcountSound)
	{
		// soundbase ��� ī��Ʈ ���� ����� ������Ʈ ���� �� �ʱ�ȭ
		playingstartComp = UGameplayStatics::SpawnSound2D(GetWorld(), startcountSound);

		// ��� ī��Ʈ ���� ��ȿ�� �˻�
		if (playingstartComp)
		{
			playingstartComp->bIsUISound = false; // ���� �ɾ��ٸ� ui ����� �������� �ʴ´�.
			playingstartComp->bAutoDestroy = true; // false : ��� �Ϸ� �� �ڵ����� �������� �ʴ´�.

			// ��� ī��Ʈ ���� ���
			playingstartComp->Play();

		}
	}
	
}

void AMarioKartPlayerController::Tick(float DeltaTime)
{
	startcountTime += DeltaTime;

	if (startcountTime >= 3.7f) // ��� ī��� ���� ��� �� ����
	{
		UE_LOG(LogTemp, Warning, TEXT("startcountTime : %.2f"), startcountTime);
		// ����, ���� ���� (����, �ӵ�)
		MoveVertical();
	}

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
				driftTime += DeltaTime;
				me->AddMovementInput(Direction(), currentSpeed);
				UE_LOG(LogTemp, Warning, TEXT("driftTime : %.2f"), driftTime);
				UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisJump ? true : false);

				// �帮��Ʈ�� 3���̻� ���ӵ��� ��
				if (driftTime >= 3.0f)
				{
					FString NumberString = FString::Printf(TEXT("driftTime: %.2f"), driftTime);
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, NumberString);
					FString DriftString = FString::Printf(TEXT("drift 3"));
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DriftString);
					// 3�� �帮��Ʈ ������ �ߵ�
					DriftActivate(3.0f);
					UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisJump ? true : false);

					// �帮��Ʈ �ð� ����
					driftTime = 0.0f;
				}
			}
			else
			{
				FVector Dir = Direction();

				if (bTestDebug)
					return;
				// ���� ���� �̵�
				me->AddMovementInput(Direction(), currentSpeed);
				//UE_LOG(LogTemp, Warning, TEXT("currentSpeed : %.2f"), currentSpeed);
			}
		}
		else
		{
			// ����Ű ������ �ʾ��� �� ������ ����
			me->AddMovementInput(Direction(), currentSpeed);
		}
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
		if (bisMovingback == true)
		{
			// ���� ȸ��
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
					// �帮��Ʈ ����
					float driftValue = FMath::Lerp(0.0f, horizontalValue, 0.7f);
					AddYawInput(driftValue);
					UE_LOG(LogTemp, Warning, TEXT("driftValue : %.2f"), driftValue)
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

	//UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());
	return returnDirection;
}

void AMarioKartPlayerController::DriftActivate(float dashActiveTime)
{
	// �÷��̾� ���̽� ���
	UGameplayStatics::PlaySound2D(GetWorld(), mariovoiceSound);

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

	UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);
	UE_LOG(LogTemp, Warning, TEXT("DriftActivate: %.2f"), dashActiveTime);

	// �帮��Ʈ ���� dashActiveTime��(Ÿ�̸� ���)
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([this]() {
		if (me->GetCharacterMovement() != nullptr)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		}

		// �뽬 ���� ���߱�뽬 ����
		if (playingAudioComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("STOP"));
 			playingAudioComp->Stop();
			playingAudioComp->SetActive(false);
			playingAudioComp = nullptr;
		}

		}), dashActiveTime, false);

	// �帮��Ʈ ����
	driftTime = 0.0f;

}

void AMarioKartPlayerController::ItemUse()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemUse"));
	ItemActivate();
}

void AMarioKartPlayerController::ItemActivate()
{
	//// �뽬 ī�޶� ����ũ
	//if (this->PlayerCameraManager != nullptr && dashShake != nullptr)
	//{
	//	
	//	this->PlayerCameraManager->StartCameraShake(dashShake);
	//	UE_LOG(LogTemp, Warning, TEXT("dashShake"));

	//}

	// �뽬 �ӵ�
	me->GetCharacterMovement()->MaxWalkSpeed = 4000.0f;

	// �뽬 1.8�� ���� ����
	GetWorldTimerManager().SetTimer(itemDelay, FTimerDelegate::CreateLambda([&]() {
		me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		}), 1.8f, false);
	UE_LOG(LogTemp, Warning, TEXT("ItemActivate"));

}


void AMarioKartPlayerController::TestDebug()
{
	bTestDebug = !bTestDebug;
}
