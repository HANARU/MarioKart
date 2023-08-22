// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "KartPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API AKartPlayer : public ANinjaCharacter
{
	GENERATED_BODY()
	
public:
	// �¿� �Է� ��
	//UPROPERTY(ReplicatedUsing = OnRep_Horizontal)

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
	class UKartPlayerAnimInstance* anim;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	float horizontalValue;

	//UFUNCTION()
	//void Horizontal();
	
	// horizontalvalue ���� ����ȭ�� ���� ����� �� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
	void ServerHorizontal();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHorizontal();

	AKartPlayer(const FObjectInitializer& ObjectInitializer);

// input ���ε� �Լ�
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//protected:
//	virtual void SetPointGravityDirection(const FVector& NewGravityPoint) override;

public:
	FTimerHandle SpeedResetTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	float Testfloat;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* kartSceneComp;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USceneComponent* kartbaseSceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UStaticMeshComponent* kartmeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UStaticMeshComponent* kartwheelComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USkeletalMeshComponent* kartCharacterBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USpringArmComponent* kartSpringComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UCameraComponent* kartCamComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USoundBase* playerDashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAudioComponent* playingDashSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Player)
		TSubclassOf<class UWidget_Player> InGamePlayerWidget;

	class UWidget_Player* UI_PlayerInGame;
	
	// �ִϸ��̼Ǹ�Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UItemComponent* playerItemComp;

	class AGM_Race* GameMode;

	int32 Current1stItem = 12;
	int32 Current2ndItem = 12;

	FString Item1stString;
	FString Item2ndString;

	/*UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 CurrentCheckpoint = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 CurrentGoalPoint = 0;*/

	UPROPERTY(ReplicatedUsing = OnRep_CurrentLapdata, VisibleAnywhere, BlueprintReadOnly, Category = Player)
		int32 CurrentGoalPoint;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentLapdata, VisibleAnywhere, BlueprintReadOnly, Category = Player)
		int32 CurrentCheckPoint;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void ReceiveItem(int32 ItemNum);

	void UsingItem();

	void ResetSpeedToNormal();
	
	//UFUNCTION()
	//void PlayAnimationMontage();

	UFUNCTION()
		void PlayAnimationMontage();

	UFUNCTION()
		void OnRep_CurrentLapdata();

	UFUNCTION()
		void ReceiveFromLapVolume(bool IsThisGoalPoint, bool IsThisCheckPoint);

	void OnCurrentLapDataUpdate();


	
};