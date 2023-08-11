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
	// �÷��̾� ĳ���� �ҷ�����
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
		if (bisAcc == true)
		{
			if (bisJump == false)
			{
				// ������ �� �̵�
				me->AddMovementInput(Direction(), currentSpeed);
			}
			else
			{
				// �帮��Ʈ ����
				// �帮��Ʈ �ð� ����
				driftTime += DeltaTime;
				me->AddMovementInput(Direction(), currentSpeed);

				if (driftTime > 1.0)
				{
					// �帮��Ʈ 
					/*bisJump = true;
					bisAcc = true;*/

					// �÷��̾� �̵� �ӵ� 2000 ���� �þ
					me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

					// �帮��Ʈ �ð� ���� ������ ������ ����
					me->AddMovementInput(Direction(), currentSpeed);
					UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);


					if (driftTime > 3.0)
					{
						// �帮��Ʈ ��
						/*bisJump = false;
						bisAcc = false;*/
						me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
						me->AddMovementInput(Direction(), currentSpeed);
						UE_LOG(LogTemp, Warning, TEXT("driftTime END"));
						UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);
						driftTime = 0.0f;
					}
				}

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


void AMarioKartPlayerController::Jump()
{
	bisJump = true;
	UE_LOG(LogTemp, Warning, TEXT("Jump: %d"), bisAcc ? true : false);
}

void AMarioKartPlayerController::Jump_released()
{
	bisJump = false;
	UE_LOG(LogTemp, Warning, TEXT("Jump_released: %d"), bisAcc ? true : false);
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

						if (driftTime > 3.0f)
						{
							// �帮��Ʈ �ð� ��
							// �⺻ ȸ������ ���ƿ�
							AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
							UCharacterMovementComponent* playerMovement = me->GetCharacterMovement();

							// �÷��̾� �̵� �ӵ� �ʱ�ȭ
							if (playerMovement != nullptr)
							{
								playerMovement->MaxWalkSpeed = 1000.0f;
							}

							// �帮��Ʈ �ð� �ʱ�ȭ
							driftTime = 0.0f;

						}
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


