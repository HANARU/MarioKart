#include "LobbyPawn.h"

ALobbyPawn::ALobbyPawn()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ALobbyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

