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
	class UTextBlock* text_players;




};
