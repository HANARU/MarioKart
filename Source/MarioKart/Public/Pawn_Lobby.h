#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Lobby.generated.h"

UCLASS()
class MARIOKART_API APawn_Lobby : public APawn
{
	GENERATED_BODY()

public:
	APawn_Lobby();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UWidget_Lobby> LobbyWidget;
	class UWidget_Lobby* Widget_Lobby;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:


};