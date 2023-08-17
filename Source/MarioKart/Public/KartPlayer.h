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
	AKartPlayer(const FObjectInitializer& ObjectInitializer);

// input 바인딩 함수
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

	class AGM_Race* GameMode;

	int32 Current1stItem = 12;
	int32 Current2ndItem = 12;

	FString Item1stString;
	FString Item2ndString;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void ReceiveItem(int32 ItemNum);

	void UsingItem();

	void ResetSpeedToNormal();

public:
	
};
