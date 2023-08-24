#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Player.generated.h"

UCLASS()
class MARIOKART_API UWidget_Player : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:

	// About GameSystem(Lap, Check, Grade)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UTextBlock* Text_CurrentCheckPoint = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UTextBlock* Text_MaxCheckPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UTextBlock* Text_CurrentLap = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UTextBlock* Text_MaxLap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UImage* IMG_Lap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Lap)
		class UImage* IMG_CurrentGrade;


	// About Collect Item(Coin, Item)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Collectable)
		class UTextBlock* Text_CurrentCoinCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_Coin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_Current1stIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_Current2ndIcon;
	
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
	class UTextBlock* text_players;
};
