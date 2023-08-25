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
	
	// �÷��̾� �ĺ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	int32 PlayerID;

	// �÷��̾ possess�� ĳ����
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* PossessedPlayer;*/


protected:
	virtual void BeginPlay() override;

	// Tick �Լ�
	virtual void Tick(float DeltaSeconds) override;

	// input ���ε� �Լ�
	void SetupInputComponent() override;


public:
	// ���� ���
	class AGM_Race* gm;
	
	// �÷��̾� ĳ����
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* me;

	// ���� �ӵ�
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Player")
	float currentSpeed;

	// �ִ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float maxSpeed = 100.0f;

	// �帮��Ʈ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float driftTime;

	// �¿� ȸ�� lerp ����, addYawInput ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float hvalue;

	//// ��� �ð� ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	//float startcountTime = 0;

	// ��������Ű �Է� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisAcc = false;

	// ��������Ű �Է� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisMovingback = false;

	// ����Ű Ȯ��(�帮��Ʈ �� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisJump = false;

	// �帮��Ʈ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisDrift = false;

	// �帮��Ʈ Ƚ�� ī��Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	int32 dashCount = 0;

	// �÷��̾� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* driveSound;

	// �÷��̾� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingdriveComp = nullptr;

	// �÷��̾� �뽬 ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* dashSound;

	// �÷��̾� �뽬 ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingAudioComp = nullptr;

	// �帮��Ʈ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* driftSound;

	// �帮��Ʈ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingdriftComp = nullptr;

	// �÷��̾� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* mariovoiceSound;

	// ��� ī��Ʈ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class USoundBase* startcountSound;

	// ��� ī��Ʈ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class UAudioComponent* playingstartComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	FVector currentDir;

	AGM_Race* RaceGM;

	// �÷��̾� �Ļ� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<class AKartPlayer> kartPlayer;

	// �۶��̵� ī��Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 glideCount = 0;

	// �۶��̵� ���� �� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "Player")	
	float velocityZ;

	// �۶��̵� �ݸ��� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player")
	bool bGlide = false;

	// �帮��Ʈ Ȱ��ȭ �Լ�
	//UFUNCTION(BlueprintImplementableEvent)
	//void DashActivate(float dashActiveTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Player")
	void Dash(float dashTime);

protected:

	// playercontroller�� kartplayer�� possess
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

	// ��� �ð� ����
	UPROPERTY()
	float startcountTime = 0;

	// �¿� �̵� �Լ�
	UFUNCTION()
	void Horizontal(float value);

	// ���� �Լ�
	UFUNCTION()
	void Acc();

	UFUNCTION()
	void Acc_released();

	// ���� �Լ�
	UFUNCTION()
	void MoveBack();

	UFUNCTION()
	void MoveBack_released();

	// �̵� ���� ���� ��ȯ �Լ�
	UFUNCTION()
	FVector Direction();

	// ����Ű �Լ�
	UFUNCTION()
	void Drift();

	UFUNCTION()
	void Drift_released();	

	// �帮��Ʈ īƮ�ٵ� ȸ�� �Լ�
	UFUNCTION()
	void DriftBody(int32 DCount);

	// �帮��Ʈ īƮ�ٵ� ���� ������� ȸ�� �Լ�
	UFUNCTION()
	void DriftBodyReturn();

	// ������ ��� �Լ�
	UFUNCTION()
	void ItemUse();

	// ������ Ȱ��ȭ �Լ�
	UFUNCTION()
	void ItemActivate();
	
	bool bTestDebug = false;
	void TestDebug();

	// �۶��̵� �Լ�
	UFUNCTION()
	void Glide();

private:
	// network role
	enum ENetRole myLocalRole;
	enum ENetRole myRemoteRole;

	UPROPERTY(Replicated)
	float timeTest = 0;

	// ���� �̵� �Լ� rpc�� �Ҵ�	
	// �����̵�(����, ����) �Լ�
	UFUNCTION()
	void MultiMoveVertical();

	void PrintLog();


	
};
