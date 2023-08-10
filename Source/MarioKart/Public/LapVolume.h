#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "I_Interact.h"
#include "LapVolume.generated.h"

UCLASS()
class MARIOKART_API ALapVolume : public AActor, public II_Interact
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		TSubclassOf<class ANinjaCharacter> PlayerBP;

	class ANinjaCharacter* PlayerSource;

	class AGM_Race* GMRace;



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Overlap_Lap_System(ANinjaCharacter* Player, bool bIsCheckPoint, bool bIsLap) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
