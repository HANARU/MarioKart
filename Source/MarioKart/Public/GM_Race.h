#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Race.generated.h"

UCLASS()
class MARIOKART_API AGM_Race : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	//void ItemOverlaped(AKartPlayer Player);
	
	void RandomItem();
};
