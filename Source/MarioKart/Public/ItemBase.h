#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

DECLARE_DELEGATE_OneParam(FItemMessageSignature, int32);

UCLASS()
class MARIOKART_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ItemBoxMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ItemBoxMark;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
		int CheckItemGotNow;

	UPROPERTY(VisibleAnywhere)
	class AGM_Race* GameMode;

	FItemMessageSignature ItemSignature;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Item, meta = (DisplayName = "Item"))
		void ItemUpdate(class AKartPlayer* OwningPlayer);

	UFUNCTION(BlueprintCallable, Category = Item)
		void CheckItemData();


	void SendItemByOverlap();
};
