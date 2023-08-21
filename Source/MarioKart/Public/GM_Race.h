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


public:
	FItemMessageSignature ItemSignature;

	void ItemOverlaped(class AKartPlayer* Player);
	void ItemOverlaped(class UItemComponent* ItemCompo);
	
	int32 RandomItem();
	int32 ItemName;

	bool Able2Play = false;
public:
    AGM_Race();

	UFUNCTION(BlueprintCallable)
	int32 GetNumberOfPlayersInLevel();

	void CheckAble2Play();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float Deltatime) override;

private:
    class UDataTable* ItemDataTable;
};
