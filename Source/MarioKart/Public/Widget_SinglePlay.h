#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_SinglePlay.generated.h"

UCLASS()
class MARIOKART_API UWidget_SinglePlay : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UWidgetSwitcher* WS_MainMenu;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UImage* IMG_Fade;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_Back2BeforeMenu;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_AccessHighlighted;

	// Index 0 : Select VS in MainMenu
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_VS_Race;

	// Index 1 : Select Player Character in UI
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_Mario;

	// Index 2 : Select Map Only MushRoom Race
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_MushRoom;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UImage* IMG_Stardium;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UImage* IMG_Rainbow;

	FName LevelIntro = FName(TEXT("0_Intro"));
	//FName Level_Loading = FName(TEXT("2_Lobby_Multi"));

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* FadeOut;
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* FadeIn;

	int32 ActivatedWidget = 0;


public:
	UFUNCTION()
		void OnClick_Back2BeforeMenu();
	UFUNCTION()
		void OnClick_AccessHighlighted();
	UFUNCTION()
		void OnClick_Press_VS_Race();
	UFUNCTION()
		void OnClick_Press_Mario();
	UFUNCTION()
		void OnClick_Press_MushRoomMap();

};
