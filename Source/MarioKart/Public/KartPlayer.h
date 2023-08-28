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
	// 좌우 입력 값
	//UPROPERTY(ReplicatedUsing = OnRep_Horizontal)

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
	class UKartPlayerAnimInstance* anim;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	float horizontalValue;

	//UFUNCTION()
	//void Horizontal();
	
	// horizontalvalue 값이 동기화로 인해 변경될 때 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ServerHorizontal();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHorizontal();

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
		TArray<class USkeletalMesh*> MeshArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class USkeletalMesh* SK_Mario;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		class USkeletalMesh* SK_Luige;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USpringArmComponent* kartSpringComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UCameraComponent* kartCameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UStaticMeshComponent* kartParachute;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class USoundBase* playerDashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAudioComponent* playingDashSound = nullptr;
	
	// 캐릭터 메쉬 번호
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="CharacterMesh")
	int32 myMeshNumber = 0;

	// 메쉬 list
	/*TArray<FString> meshPathList = {TEXT("/Game/4_SK/Mario/Mesh/Mario"),
									TEXT("/Game/4_SK/Mario/Mesh/Mario_Blue"), 
									TEXT("/Game/4_SK/Luige/Mesh/Luige"),
									TEXT("/Game/4_SK/Luige/Mesh/Luige_Yellow")};*/
	TArray<FString> meshPathList = {TEXT("/Game/4_SK/Luige/Mesh/Mario_Blue"),
									TEXT("/Game/4_SK/Luige/Mesh/Mario"), 
									TEXT("Game/4_SK/Luige/Mesh/Mario_Blue"),
									TEXT("Game/4_SK/Luige/Mesh/Mario")};


	//// 애니메이션 블루프린트 list
	//TArray<FString> animPathList = { TEXT("/Game/4_SK/Mario/Animation/ABP_Mario"),
	//								TEXT("/Game/4_SK/Mario/Animation/ABP_MarioPlayerBlue"),
	//								TEXT("/Game/4_SK/Luige/Animation/ABP_Luige"),
	//								TEXT("/Game/4_SK/Luige/Animation/ABP_LuigeYellow") };

	UPROPERTY(EditDefaultsOnly, Category = Player)
		TSubclassOf<class UWidget_Player> InGamePlayerWidget;

	class UWidget_Player* UI_PlayerInGame;
	
	// 애니메이션몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UAnimMontage* M_Base;

	UFUNCTION(NetMulticast, Unreliable)
		void Multicast_PlayAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UItemComponent* playerItemComp;

	class AGM_Race* GameMode;
	class AState_KartPlayer* KartPlayerState;
	class UKartInstance* KartInstance;
	int32 PlayerNumber = 12;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentItemData, VisibleAnywhere, BlueprintReadWrite, Category = Item)
		int32 Current1stItem = 12;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentItemData, VisibleAnywhere, BlueprintReadWrite, Category = Item)
		int32 Current2ndItem = 12;

	FString Item1stString;
	FString Item2ndString;
	FString LocalItemDataMessage;
	FString ServerItemDataMessage;

	UPROPERTY(EditAnywhere, Category = Player)
	bool ItemDataLog = false;


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void PrintStringAtPlayer();

public:
	UFUNCTION()
	void OnRep_CurrentItemData();

	void OnCurrentItemDataUpdate();

	UFUNCTION()
	void ReceiveItem(int32 ItemNum);

	void UsingItem();

	//UFUNCTION()
	//void PlayAnimationMontage();

	UFUNCTION()
		void PlayAnimationMontage();

	void ResetSpeedToNormal();
	
	// 캐릭터 메쉬 설정 함수
	UFUNCTION(Server, Reliable)
	void ServerSetInfo(int32 playernumber);

private:

	void InitializePlayer();

};