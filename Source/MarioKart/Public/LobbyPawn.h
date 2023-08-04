#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyPawn.generated.h"

UCLASS()
class MARIOKART_API ALobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	ALobbyPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
