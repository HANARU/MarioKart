#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Lobby.generated.h"

UCLASS()
class MARIOKART_API UWidget_Lobby : public UUserWidget
{
	GENERATED_BODY()

protected:
		virtual void NativeConstruct() override;
public:
 //   SLATE_BEGIN_ARGS(UWidget_Lobby) {}

	//SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	//SLATE_END_ARGS()

	//void Construct(const FArguments& InArgs);

	//TWeakObjectPtr<class AMenuHUD> OwningHUD;

	//virtual bool SupportsKeyboardFocus() const override { return true; };

	

	// Index 0 : Select Button to Check PlayerCount
	/*UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UWidgetSwitcher* WS_Intro;*/
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_SinglePlay;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_Check;
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
	//	class UButton* Button_MultiPlay;

	//// Index 1 : Select Button to Widget Interact
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MenuSystem)
	//	class UWidgetSwitcher* WS_SingleMenu;
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MenuSystem)
	//	class UButton* Button_Back2Before;
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MenuSystem)
	//	class UButton* Button_Move2Next;

	//// Index 1-1 : Select Button In SinglePlay
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu1st)
	//	class UButton* Button_VS_Race_Single;

	//// Index 1-2 : Select Button In MultiPlay
	//UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu1st)
	//	class UButton* Button_VS_Race_LocalNetwork;

	UFUNCTION(BlueprintCallable)
	void ClickedSinglePlay();

	UFUNCTION()
	void ClickedCheck();

	
	
};
