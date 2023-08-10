#include "KartPlayer.h"
#include "NinjaCharacter.h"
#include "NinjaCharacterMovementComponent.h"

AKartPlayer::AKartPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNinjaCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	Testfloat = 5;
}
