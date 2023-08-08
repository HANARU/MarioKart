#include "Pawn_Lobby.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_Lobby.h"

APawn_Lobby::APawn_Lobby()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void APawn_Lobby::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SetShowMouseCursor(true);

	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		Widget_Lobby = CreateWidget<UWidget_Lobby>(GetWorld(), LobbyWidget);
		if (Widget_Lobby != nullptr)
		{
			Widget_Lobby->AddToViewport();
		}
	}
}

void APawn_Lobby::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawn_Lobby::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

