// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
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
	
	// 플레이어 식별자 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	int32 PlayerID;

	// 플레이어가 possess한 캐릭터
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* PossessedPlayer;*/


protected:
	virtual void BeginPlay() override;

	// Tick 함수
	virtual void Tick(float DeltaSeconds) override;

	// input 바인딩 함수
	void SetupInputComponent() override;


public:
	// 게임 모드
	class AGM_Race* gm;
	
	// 플레이어 캐릭터
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* me;

	// 현재 속도
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Player")
	float currentSpeed;

	// 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float maxSpeed = 100.0f;

	// 드리프트 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float driftTime;

	// 좌우 회전 lerp 변수, addYawInput 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float hvalue;

	//// 출발 시간 저장
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	//float startcountTime = 0;

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

	// 드리프트 횟수 카운트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	int32 dashCount = 0;

	// 플레이어 주행 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* driveSound;

	// 플레이어 주행 사운드 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingdriveComp = nullptr;

	// 플레이어 대쉬 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* dashSound;

	// 플레이어 대쉬 사운드 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingAudioComp = nullptr;

	// 드리프트 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* driftSound;

	// 드리프트 사운드 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingdriftComp = nullptr;

	// 플레이어 마리오 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* mariovoiceSound;

	// 출발 카운트 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* startcountSound;

	// 출발 카운트 사운드 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingstartComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	FVector currentDir;

	AGM_Race* RaceGM;

	// 플레이어 파생 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<class AKartPlayer> kartPlayer;

	// 글라이드 카운트
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 glideCount = 0;

	// 글라이드 시작 전 속도
	UPROPERTY(EditDefaultsOnly, Category = "Player")	
	float velocityZ;

	// 글라이드 콜리젼 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player")
	bool bGlide = false;

	// 드리프트 활성화 함수
	//UFUNCTION(BlueprintImplementableEvent)
	//void DashActivate(float dashActiveTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void Dash(float dashTime);

protected:

	// playercontroller를 kartplayer에 possess
	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(Server, Reliable)
	void ServerOnPossess(APawn* aPawn);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastOnPossess(APawn* aPawn);

	//UFUNCTION(Server, Reliable)
	//void ServerPossessPawn();

private:
	bool bInDelay = false;
	FTimerHandle itemDelay;
	FTimerHandle glideDelay;

	// 출발 시간 저장
	UPROPERTY()
	float startcountTime = 0;

	// 좌우 이동 함수
	UFUNCTION()
	void Horizontal(float value);

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

	// 이동 방향 벡터 반환 함수
	UFUNCTION()
	FVector Direction();

	// 점프키 함수
	UFUNCTION()
	void Drift();

	UFUNCTION()
	void Drift_released();	

	// 드리프트 카트바디 회전 함수
	UFUNCTION()
	void DriftBody(int32 DCount);

	// 드리프트 카트바디 원래 모습으로 회전 함수
	UFUNCTION()
	void DriftBodyReturn();

	// 아이템 사용 함수
	UFUNCTION()
	void ItemUse();

	// 아이템 활성화 함수
	UFUNCTION()
	void ItemActivate();
	
	bool bTestDebug = false;
	void TestDebug();

	// 글라이딩 함수
	UFUNCTION()
	void Glide();

private:
	// network role
	enum ENetRole myLocalRole;
	enum ENetRole myRemoteRole;

	UPROPERTY(Replicated)
	float timeTest = 0;

	// 상하 이동 함수 rpc로 할당	
	// 상하이동(전진, 후진) 함수
	UFUNCTION()
	void MultiMoveVertical();

	void PrintLog();


	
};
