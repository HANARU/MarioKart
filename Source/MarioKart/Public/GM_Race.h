#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ItemInfo.h"
#include "GM_Race.generated.h"

DECLARE_DELEGATE_OneParam(FItemMessageSignature, int32);

UCLASS()
class MARIOKART_API AGM_Race : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	FItemMessageSignature ItemSignature;

	void ItemOverlaped(class AKartPlayer* Player);
	void ItemOverlaped(class UItemComponent* ItemCompo);
	
	int32 RandomItem();
	int32 ItemName;
public:
    AGM_Race();

private:
    class UDataTable* ItemDataTable;
};
