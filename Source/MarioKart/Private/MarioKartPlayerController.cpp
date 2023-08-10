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
	// 전진, 후진 주행 (가속, 속도)
	MoveVertical();

	if (bisMovingback)
	{
		// 후진할 때 이동
		me->AddMovementInput(Direction(), currentSpeed);
	}
	else
	{
		if (!bisAcc)
		{
			// 가속키 누르지 않았을 때 서서히 감속
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
				// 드리프트 시간 누적
				driftTime+=DeltaTime;
				
				if (driftTime > 1.0)
				{
					// 드리프트 
					bisJump = true;
					bisAcc = true;

					// 플레이어 이동 속도 2000 으로 늘어남
					me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

					// 드리프트 시간 동안 앞으로 빠르게 전진
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
	//입력 함수 바인딩

	// 좌우 입력
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMarioKartPlayerController::Horizontal);

	// 가속키 입력
	PlayerInputComponent->BindAction(TEXT("Acc"), IE_Pressed, this, &AMarioKartPlayerController::Acc);

	// 후진키 입력
	PlayerInputComponent->BindAction(TEXT("MoveBack"), IE_Pressed, this, &AMarioKartPlayerController::MoveBack);

	// 점프키 입력
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
	// 좌우 입력값
	horizontalValue = value;
	
	// 좌우 입력 들어왔을 때(회전)
	if (horizontalValue != 0)
	{
		// 후진 회전
		if (bisMovingback)
		{
			AddYawInput(FMath::Lerp(horizontalValue, 0.0f, 0.3f));
		}
		else
		{
			// 전진 회전
			if (bisAcc)
			{
				// 드리프트(점프 키 입력)
				if (bisJump)
				{
					// 드리프트 발동 중
					// 회전율(Alpha) 더 높게 설정
					AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.7f));

					// 드리프트 시간 끝나고
					if (driftTime > 3.0f)
					{
						// 기본 회전으로 돌아옴
						AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
						UCharacterMovementComponent* playerMovement = me->GetCharacterMovement();
						
						// 플레이어 이동 속도 초기화
						if (playerMovement)
						{
							playerMovement->MaxWalkSpeed = 1000.0f;
						}

						// 드리프트 시간 초기화
						driftTime = 0.0f; 

					}
					else // 드리프트 발동 회전
					{
						AddYawInput(FMath::Lerp(0.0f, 0.5f, 0.01f));
						
					}

				}
				else // 드리프트 해제(점프키 땠을 때)
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
			// 전진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed, GetWorld()->GetDeltaSeconds() * 1.5);

		}
		else
		{
			// 후진 가속
			currentSpeed = FMath::Lerp(currentSpeed, maxSpeed * -0.5f, GetWorld()->GetDeltaSeconds());
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
	// 캐릭터의 getworldrotation은 
	// ACharacter 클래스가 상속한 APawn 클래스의 멤버 함수인 GetActorRotation을 사용
	FRotator worldRotation = me->GetActorRotation();

	// 주행 방향 구하기
	FVector crossVector = UKismetMathLibrary::Cross_VectorVector(UKismetMathLibrary::GetRightVector(GetControlRotation()), UKismetMathLibrary::GetUpVector(worldRotation));
	FVector returnDirection = UKismetMathLibrary::Normal(crossVector, 0.0001);

	return returnDirection;
}


