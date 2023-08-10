#include "KartPlayerAnimInstance.h"
#include "KartTestModel.h"

void UKartPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<AKartTestModel>(TryGetPawnOwner());
}

void UKartPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Player != nullptr)
	{
		HorizontalValue = Player->HorizontalValue;
	}
}
