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

	
	int32 RandomItem();
	int32 ItemName;
	int32 PlayerCount = 0;

	bool Able2Play = false;
	int MaxPlayerNum = 4;
	
public:
    AGM_Race();

	void CheckAble2Play();

	UFUNCTION(BlueprintNativeEvent)
		void StartCountDown();

	void ItemOverlaped(class AKartPlayer* Player);
	void ItemOverlaped(class UItemComponent* ItemComp);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float Deltatime) override;

private:
    class UDataTable* ItemDataTable;
};
