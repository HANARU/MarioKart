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

	UFUNCTION()
	void OnRep_HorizontalValue();

private:
	UPROPERTY()
	class AKartPlayer* kartPlayer;

	UPROPERTY()
	class AMarioKartPlayerController* kartplayerController;

	UPROPERTY()
	class UCharacterMovementComponent* moveComp;
};
