#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KartPlayerAnimInstance.generated.h"


UCLASS()
class MARIOKART_API UKartPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	class AKartTestModel* Player;

	UPROPERTY(ReplicatedUsing = OnRep_HorizontalValue, EditDefaultsOnly, BlueprintReadWrite)
	float HorizontalValue;

	// 애니메이션 블루프린트 list
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	//TArray<FString> animPathList = {TEXT("/Game/4_SK/Mario/Animation/ABP_Mario"),
	//								TEXT("/Game/4_SK/Mario/Animation/ABP_MarioPlayerBlue"),
	//								TEXT("/Game/4_SK/Luige/Animation/ABP_Luige"),
	//								TEXT("/Game/4_SK/Luige/Animation/ABP_LuigeYellow") };

		TArray<FString> animPathList = { TEXT("/Game/4_SK/Luige/Animation/ABP_Luige"),
									TEXT("/Game/4_SK/Luige/Animation/ABP_LuigeYellow"),
									TEXT("/Game/4_SK/Mario/Animation/ABP_Mario"),
									TEXT("/Game/4_SK/Mario/Animation/ABP_MarioPlayerBlue")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimBlueprint* ABP_anim;

	UFUNCTION()
	void OnRep_HorizontalValue();

	UFUNCTION()
	void SetAnimation(int32 animNum);

private:
	UPROPERTY()
	class AKartPlayer* kartPlayer;

	UPROPERTY()
	class AMarioKartPlayerController* kartplayerController;

	UPROPERTY()
	class UCharacterMovementComponent* moveComp;
};
