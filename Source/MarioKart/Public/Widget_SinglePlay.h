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
		class UButton* Button_Back2BeforeMenu;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_AccessHighlighted;

	// Index 0 : Select VS in MainMenu
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_VS_Race;


public:
	UFUNCTION()
		void OnClick_Back2BeforeMenu();
	UFUNCTION()
		void OnClick_AccessHighlighted();
};
