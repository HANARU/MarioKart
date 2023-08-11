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
	// 플레이어 캐릭터 불러오기
	me = Cast<AKartPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	FString NameString = UKismetStringLibrary::Conv_ObjectToString(me);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, NameString);
}

void AMarioKartPlayerController::Tick(float DeltaTime)
{
	// 전진, 후진 주행 (가속, 속도)
	MoveVertical();

	if (bisMovingback == true)
	{
		// 후진할 때 이동
		me->AddMovementInput(Direction(), currentSpeed);
	}
	else
	{
		if (bisAcc == true)
		{
			if (bisJump == false)
			{
				// 전진할 때 이동
				me->AddMovementInput(Direction(), currentSpeed);
			}
			else
			{
				// 드리프트 시작
				// 드리프트 시간 누적
				driftTime += DeltaTime;
				me->AddMovementInput(Direction(), currentSpeed);

				if (driftTime > 1.0)
				{
					// 드리프트 
					/*bisJump = true;
					bisAcc = true;*/

					// 플레이어 이동 속도 2000 으로 늘어남
					me->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;

					// 드리프트 시간 동안 앞으로 빠르게 전진
					me->AddMovementInput(Direction(), currentSpeed);
					UE_LOG(LogTemp, Warning, TEXT("driftTime: %.2f"), driftTime);


					if (driftTime > 3.0)
					{
						// 드리프트 끝
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

						if (driftTime > 3.0f)
						{
							// 드리프트 시간 끝
							// 기본 회전으로 돌아옴
							AddYawInput(FMath::Lerp(0.0f, horizontalValue, 0.3f));
							UCharacterMovementComponent* playerMovement = me->GetCharacterMovement();

							// 플레이어 이동 속도 초기화
							if (playerMovement != nullptr)
							{
								playerMovement->MaxWalkSpeed = 1000.0f;
							}

							// 드리프트 시간 초기화
							driftTime = 0.0f;

						}
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
	// 캐릭터의 getworldrotation
	// ACharacter 클래스가 상속한 APawn 클래스의 멤버 함수인 GetActorRotation을 사용
	FRotator worldRotation = me->GetActorRotation();
	//FRotator::ZeroRotator;

	// 주행 방향 구하기
	FVector crossVector = UKismetMathLibrary::Cross_VectorVector(UKismetMathLibrary::GetRightVector(GetControlRotation()), UKismetMathLibrary::GetUpVector(worldRotation));
	FVector returnDirection = UKismetMathLibrary::Normal(crossVector, 0.0001);

	UE_LOG(LogTemp, Warning, TEXT("Direction %s"), *returnDirection.ToString());

	return returnDirection;
}


