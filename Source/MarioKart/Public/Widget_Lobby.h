#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Lobby.generated.h"

UCLASS()
class MARIOKART_API UWidget_Lobby : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UWidgetSwitcher* WS_Intro;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UImage* IMG_InitFade;

	// Index 0 : Select Button to Check PlayerCount
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_PressStartAtIntro;

	FName LevelSingle = FName(TEXT("1_Lobby_Single"));
	FName LevelMulti = FName(TEXT("2_Lobby_Multi"));

	// Index 1 : Play SinglePlay or Multiplay
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_PressSinglePlay;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_PressMultiPlay;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UImage* IMG_NextMenu;

	// Animation Bind by C++
	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* Intro2Menu;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* MainMenuStart;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* Menu2RaceType;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* IntroInit;

	UFUNCTION()
		void OnClickedPressIntro2Menu();

	UFUNCTION()
		void OnClickedPressMenu2Single();

	UFUNCTION()
		void OnClickedPressMenu2Multi();
	
};
