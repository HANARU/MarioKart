#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KartPlayer.h"
#include "LapVolume.generated.h"

UCLASS()
class MARIOKART_API ALapVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ALapVolume();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSystem)
		class UBoxComponent* CheckVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSystem)
		bool bIsThisCheckPoint = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameSystem)
		bool bIsThisLapPoint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lap)
		int LapCheckValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		TSubclassOf<class AKartPlayer> PlayerBP;

	class AGM_Race* GMRace;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lap, meta = (DisplayName = "Lap"))
		void LapUpdate(class AKartPlayer* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = Lap)
		void CheckLapData();

};
