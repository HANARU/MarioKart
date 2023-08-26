// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GameStart.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API UWidget_GameStart : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
	class UButton* btn_GameStart;

	// Animation Bind by C++
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
	class UWidgetAnimation* TrophyBlankFade;

	FName LevelMultiStardium = FName(TEXT("7_Multi_Stardium"));

	UFUNCTION()
	void OnClickCreateButton();

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UTextBlock* text_player_1;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UTextBlock* text_player_2;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UTextBlock* text_player_3;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UTextBlock* text_player_4;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_MarioReady;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_LuigeReady;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_DaisyReady;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Collectable)
		class UImage* IMG_YoshiReady;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	TArray<FText> PlayerArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
	TArray<float> RenderArray;

	UPROPERTY()
	FString PlayerInfoText1;
	UPROPERTY()
	FString PlayerInfoText2;
	UPROPERTY()
	FString PlayerInfoText3;
	UPROPERTY()
	FString PlayerInfoText4;
};
