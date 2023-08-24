#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "State_KartPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLapChanged, int, NewLapValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckChanged, int, NewCheckValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn1stItem, int, New1stItem);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn2ndItem, int, New2ndItem);

UCLASS()
class MARIOKART_API AState_KartPlayer : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_Lap, Category = Lap)
		int LapValue = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_Check, Category = Lap)
		int CheckValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lap)
		int CurrentLapPoint = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lap)
		int CurrentCheckPoint = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_Item1st, Category = Item)
		int Item1st = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lap)
		int Current1stItem = 12;

	//UPROPERTY()

	UFUNCTION()
		void OnRep_Lap(int OldLap);
	UFUNCTION()
		void OnRep_Check(int OldCheck);
	UFUNCTION()
		void OnRep_Item1st(int OldValue);

public:
	UFUNCTION(BlueprintCallable, Category = Lap)
		void AddLapData(int Value);

	UPROPERTY(BlueprintAssignable, Category = Events)
		FOnLapChanged OnLapChanged;
	UPROPERTY(BlueprintAssignable, Category = Events)
		FOnCheckChanged OnCheckChanged;

	UFUNCTION(BlueprintCallable, Category = Item)
		void Add1stItem(int Value);
	//UFUNCTION(BlueprintCallable, Category = Item)
	//	void Add2ndItem(int Value);

	UPROPERTY(BlueprintAssignable, Category = Events)
		FOn1stItem On1stItem;
	//UPROPERTY(BlueprintAssignable, Category = Events)
	//	FOn2ndItem On2ndItem;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
