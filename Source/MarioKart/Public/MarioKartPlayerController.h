// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KartCharacter.h"
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
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent);


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

	// ��������Ű �Է� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisAcc = false;

	// ��������Ű �Է� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisMovingback = false;

	// ����Ű Ȯ��(�帮��Ʈ �� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisJump = false;

	// �帮��Ʈ Ű Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool bisDrift = false;

	// �÷��̾� ĳ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	AKartCharacter* me;


	// ���� �Լ�
	void Acc();

	// ���� �Լ�
	void MoveBack();

	// ����Ű �Լ�
	void Jump();

	// �¿� �̵� �Լ�
	void Horizontal(float value);

	// �����̵�(����, ����) �Լ�
	UFUNCTION()
	void MoveVertical();

	// �̵� ���� ���� ��ȯ �Լ�
	UFUNCTION()
	FVector Direction();

};
