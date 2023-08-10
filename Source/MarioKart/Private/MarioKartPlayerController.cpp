// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioKartPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AMarioKartPlayerController::AMarioKartPlayerController()
{
	PrimaryActorTick.bCanEverTick = true; 

}

void AMarioKartPlayerController::BeginPlay()
{
	me = Cast<AKartCharacter>(GetOwner());
}

void AMarioKartPlayerController::Tick(float DeltaTime)
{
	// ����, ���� ���� (����, �ӵ�)
	MoveVertical();

	if (bisMovingback)
	{
		// ������ �� �̵�
		me->AddMovementInput(Direction(), currentSpeed);
	}
	else
	{
		if (!bisAcc)
		{
			// ����Ű ������ �ʾ��� �� ������ ����
			me->AddMovementInput(Direction(), currentSpeed);
		}
		else
		{
			if (!bisJump)
			{
				me->AddMovementInput(Direction(), currentSpeed);
			}
			else
			{
				// �帮��Ʈ �ð� ����
				driftTime+=DeltaTime;
				
				if (driftTime > 1.0)
				{
					// �帮��Ʈ 
					bisJump = true;
					bisAcc = true;

					// �÷��̾� �̵� �ӵ� 2000 ���� �þ
					me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

					// �帮��Ʈ �ð� ���� ������ ������ ����
					me->AddMovementInput(Direction(), currentSpeed);

					if (driftTime > 3.0)
					{
						bisJump = false;
						bisAcc = false;
						me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
						me->AddMovementInput(Direction(), currentSpeed);
						driftTime = 0.0f;
					}
					
				}
			}
		}
	}



}

void AMarioKartPlayerController::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	//�Է� �Լ� ���ε�

	// �¿� �Է�
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMarioKartPlayerController::Horizontal);

	// ����Ű �Է�
	PlayerInputComponent->BindAction(TEXT("Acc"), IE_Pressed, this, &AMarioKartPlayerController::Acc);

	// ����Ű �Է�
	PlayerInputComponent->BindAction(TEXT("MoveBack"), IE_Pressed, this, &AMarioKartPlayerController::MoveBack);

	// ����Ű �Է�
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMarioKartPlayerController::Jump);
}

void AMarioKartPlayerController::Acc()
{
	bisAcc = true;
}

void AMarioKartPlayerController::MoveBack()
{
	bisMovingback = true;

}

void AMarioKartPlayerController::Jump()
{
	bisJump = true;
}

void AMarioKartPlayerController::Horizontal(float value)
{
	// �¿� �Է°�
	horizontalValue = value;
	
	// �¿� �Է� ������ ��(ȸ��)
	if (horizontalValue != 0)
	{
		// ���� ȸ��
		if (bisMovingback)
		{
			AddYawInput(FMath::Lerp(horizontalValue, 0.0f, 0.3f));
		}
		else
		{
			// ���� ȸ��
			if (bisAcc)
			{
				// �帮��Ʈ(���� Ű �Է�)
				if (bisJump)
				{
					// �帮��Ʈ �ߵ� ��
					// ȸ����(Alpha) �� ���� ����
					AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.7f));

					// �帮��Ʈ �ð� ������
					if (driftTime > 3.0f)
					{
						// �⺻ ȸ������ ���ƿ�
						AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
						UCharacterMovementComponent* playerMovement = me->GetCharacterMovement();
						
						// �÷��̾� �̵� �ӵ� �ʱ�ȭ
						if (playerMovement)
						{
							playerMovement->MaxWalkSpeed = 1000.0f;
						}

						// �帮��Ʈ �ð� �ʱ�ȭ
						driftTime = 0.0f; 

					}
					else // �帮��Ʈ �ߵ� ȸ��
					{
						AddYawInput(FMath::Lerp(0.0f, 0.5f, 0.01f));
						
					}

				}
				else // �帮��Ʈ ����(����Ű ���� ��)
				{
					AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));

				}
			}
		}
	}

}

void AMarioKartPlayerController::MoveVertical()
{
	if (bisAcc || bisMovingback)
	{
		if (bisAcc)
		{
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5);

		}
		else
		{
			// ���� ����
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed * -0.5f, GetWorld()->GetDeltaSeconds());
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
	// ĳ������ getworldrotation�� 
	// ACharacter Ŭ������ ����� APawn Ŭ������ ��� �Լ��� GetActorRotation�� ���
	FRotator worldRotation = me->GetActorRotation();

	// ���� ���� ���ϱ�
	FVector crossVector = UKismetMathLibrary::Cross_VectorVector(UKismetMathLibrary::GetRightVector(GetControlRotation()), UKismetMathLibrary::GetUpVector(worldRotation));
	FVector returnDirection = UKismetMathLibrary::Normal(crossVector, 0.0001);

	return returnDirection;
}


