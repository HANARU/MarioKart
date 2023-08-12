#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "I_Interact.h"
#include "GM_Race.generated.h"

UCLASS()
class MARIOKART_API AGM_Race : public AGameModeBase, public II_Interact
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void Overlap_Lap_System(ANinjaCharacter* Player, bool bIsCheckPoint, bool bIsLap) override;
	

};
