// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioKartPlayerController.h"
#include "KartPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AMarioKartPlayerController::AMarioKartPlayerController()
{
	PrimaryActorTick.bCanEverTick = true; 

}

void AMarioKartPlayerController::BeginPlay()
{
	me = Cast<AKartPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);
}

void AMarioKartPlayerController::Tick(float DeltaTime)
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
		if (bisAcc == false)
		{
			// ����Ű ������ �ʾ��� �� ������ ����
			me->AddMovementInput(Direction(), currentSpeed);

		}
		else
		{
			// ������ �� �̵�
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
	UE_LOG(LogTemp, Warning, TEXT("MoveBack: %d"), bisAcc ? true : false);
}

void AMarioKartPlayerController::MoveBack_released()
{
	bisMovingback = false;
	UE_LOG(LogTemp, Warning, TEXT("MoveBack_released: %d"), bisAcc ? true : false);
}



void AMarioKartPlayerController::Horizontal(float value)
{
	// �¿� �Է°�
	horizontalValue = value;
	
	// �¿� �Է� ������ ��(ȸ��)
	if (FMath::Abs(horizontalValue) != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("horizontal rotation"));

		// ���� ȸ��
		if (bisMovingback == true)
		{
			float movigbackValue = FMath::Lerp(horizontalValue, 0.0f, 0.3f);
			AddYawInput(movigbackValue);
			UE_LOG(LogTemp, Warning, TEXT("movigbackValue : %.2f"), movigbackValue);

		}
		
		if(bisAcc == true)
		{
			// ���� ȸ��
			AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
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
	// ĳ������ getworldrotation
	// ACharacter Ŭ������ ����� APawn Ŭ������ ��� �Լ��� GetActorRotation�� ���
	FRotator worldRotation = me->GetActorRotation();
	//FRotator::ZeroRotator;

	// ���� ���� ���ϱ�
	FVector crossVector = UKismetMathLibrary::Cross_VectorVector(UKismetMathLibrary::GetRightVector(GetControlRotation()), UKismetMathLibrary::GetUpVector(worldRotation));
	FVector returnDirection = UKismetMathLibrary::Normal(crossVector, 0.0001);

	UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());

	return returnDirection;
}


