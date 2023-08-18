#include "KartPlayerAnimInstance.h"
#include "KartTestModel.h"
#include "KartPlayer.h"
#include "MarioKartPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"


void UKartPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//Player = Cast<AKartTestModel>(TryGetPawnOwner());
	//kartPlayer = Cast<AKartPlayer>(GetOwningActor());
	kartPlayer = Cast<AKartPlayer>(TryGetPawnOwner());
	if (kartPlayer != nullptr)
	{
		moveComp = kartPlayer->GetCharacterMovement();
	}
}

void UKartPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*if (Player != nullptr)
	{
		HorizontalValue = Player->HorizontalValue;
	}*/
	if (kartPlayer != nullptr)
	{
		HorizontalValue = kartPlayer->horizontalValue;
		//UE_LOG(LogTemp, Warning, TEXT("NetMode : %d, Connection : %s"), TryGetPawnOwner()->GetNetMode(), TryGetPawnOwner()->GetNetConnection() == nullptr ? TEXT("NULL") : TEXT("Valid"));

		AMarioKartPlayerController* myPlayerController = Cast<AMarioKartPlayerController>(kartPlayer->GetController());
		if (myPlayerController)
		{
			
		}
	}
}
