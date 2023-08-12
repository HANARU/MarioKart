// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MarioKartPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API AMarioKartPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMarioKartPlayerController();
	

protected:
	virtual void BeginPlay() override;

public:
	// Tick 함수
	virtual void Tick(float DeltaTime) override;

	// input 바인딩 함수
	void SetupInputComponent() override;


public:
	
	// 좌우 입력 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float horizontalValue;

	// 현재 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float currentSpeed;

	// 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float maxSpeed = 100.0f;

	// 드리프트 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float driftTime;

	// 전진주행키 입력 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisAcc = false;

	// 후진주행키 입력 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisMovingback = false;

	// 점프키 확인(드리프트 때 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisJump = false;

	// 드리프트 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisDrift = false;

	// 플레이어 캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* me;


private:
	bool bInDelay = false;
	FTimerHandle itemDelay;

<<<<<<< Updated upstream
	UPROPERTY()
	float itemInterval = 0.0f;


=======
>>>>>>> Stashed changes
	// 전진 함수
	UFUNCTION()
	void Acc();

	UFUNCTION()
	void Acc_released();

	// 후진 함수
	UFUNCTION()
	void MoveBack();

	UFUNCTION()
	void MoveBack_released();

	// 점프키 함수
	UFUNCTION()
	void Jump();

	UFUNCTION()
	void Jump_released();

	// 좌우 이동 함수
	UFUNCTION()
	void Horizontal(float value);

	// 상하이동(전진, 후진) 함수
	UFUNCTION()
	void MoveVertical();

	// 이동 방향 벡터 반환 함수
	UFUNCTION()
	FVector Direction();

	// 드리프트 활성화 함수
	UFUNCTION()
	void DriftActivate(float dashActiveTime);

	// 아이템 사용 함수
	UFUNCTION()
<<<<<<< Updated upstream
	void ItemUse();

	// 아이템 활성화 함수
	UFUNCTION()
	void ItemActivate();

=======
	void Item();
	
>>>>>>> Stashed changes
	bool bTestDebug = false;
	void TestDebug();

};
