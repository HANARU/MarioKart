#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARIOKART_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



class AGM_Race* GameMode;

public:
	UPROPERTY(/*ReplicatedUsing = OnRep_CurrentItemData,*/ VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 Current1stItem = 12;
	UPROPERTY(/*ReplicatedUsing = OnRep_CurrentItemData,*/ VisibleAnywhere, BlueprintReadOnly, Category = Item)
	int32 Current2ndItem = 12;

	FString Item1;
	FString Item2;

	
public:
	UFUNCTION()
	void ReceiveItemfromCharacter(int32 ItemNum);

	void UsingItem();

	UFUNCTION()
	void OnRep_CurrentItemData();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
