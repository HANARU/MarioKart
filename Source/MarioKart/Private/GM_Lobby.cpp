#include "GM_Lobby.h"
#include "Pawn_Lobby.h"

AGM_Lobby::AGM_Lobby()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Game/2_BP/BP_Pawn_Lobby"));
	if (PlayerPawnClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}
