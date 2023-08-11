#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverlayActor.generated.h"

UCLASS()
class MARIOKART_API AOverlayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOverlayActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Render)
		class USceneComponent* BaseScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Render)
		class UStaticMeshComponent* KartBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Render)
		class USkeletalMeshComponent* PlayerBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Render)
		class USpotLightComponent* Light;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Render)
		class USceneCaptureComponent2D* Capture;

	int32 KartNumber = 0;
	int32 PlayerNumber = 0;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
