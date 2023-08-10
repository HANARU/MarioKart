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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float HorizontalValue;
};
