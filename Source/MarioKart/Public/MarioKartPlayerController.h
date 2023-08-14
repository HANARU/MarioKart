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
	// Tick �Լ�
	virtual void Tick(float DeltaTime) override;

	// input ���ε� �Լ�
	void SetupInputComponent() override;


public:
	
	// �¿� �Է� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float horizontalValue;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float currentSpeed;

	// �ִ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float maxSpeed = 100.0f;

	// �帮��Ʈ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float driftTime;

	// ��� �ð� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	float startcountTime = 0.0f;

	//// �뽬 ī�޶� ����ũ
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player")
	//TSubclassOf<class UCameraShakeBase> dashShake;

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

	// �÷��̾� ĳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	class AKartPlayer* me;

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

private:
	bool bInDelay = false;
	FTimerHandle itemDelay;

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

	// ����Ű �Լ�
	UFUNCTION()
	void Jump();

	UFUNCTION()
	void Jump_released();

	// �¿� �̵� �Լ�
	UFUNCTION()
	void Horizontal(float value);

	// �����̵�(����, ����) �Լ�
	UFUNCTION()
	void MoveVertical();

	// �̵� ���� ���� ��ȯ �Լ�
	UFUNCTION()
	FVector Direction();

	// �帮��Ʈ Ȱ��ȭ �Լ�
	UFUNCTION()
	void DriftActivate(float dashActiveTime);

	// ������ ��� �Լ�
	UFUNCTION()
	void ItemUse();

	// ������ Ȱ��ȭ �Լ�
	UFUNCTION()
	void ItemActivate();
	
	bool bTestDebug = false;
	void TestDebug();

};
