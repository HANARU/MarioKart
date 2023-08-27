#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KartInstance.h"
#include "Widget_MultiPlay.generated.h"

UCLASS()
class MARIOKART_API UWidget_MultiPlay : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UWidgetSwitcher* WS_MainMenu;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = Menu)
		class UImage* IMG_Fade;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_Back2BeforeMenu;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_AccessHighlighted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		class UAudioComponent* BGMComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Menu)
		class USoundBase* MenuBGM;

	//UserName 입력
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UEditableText* EText_UserName;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_InputUserName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_MakeLobby;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UEditableText* EText_RoomName;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UVerticalBox* VB_SessionList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_FindLobby;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* FadeOut;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim), Category = Menu)
		class UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere, Category = MySettings)
		TSubclassOf<class UWidget_SessionInfo> sessionInfoWidget;

		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio") // 사운드 리소스 선언
    class USoundBase* ClickSound;

	FName LevelIntro = FName(TEXT("0_Intro"));
	int32 ActivatedWidget = 0;

public:
	UFUNCTION()
		void OnClick_Back2BeforeMenu();
	UFUNCTION()
		void OnClick_AccessHighlighted();
    
	UFUNCTION()
	    void OnClick_NextInputName();

	UFUNCTION()
		void OnClick_CreateLobby();
	UFUNCTION()
		void OnClick_FindLobby();
	UFUNCTION()
		void AddRoomSlot(FSessionSlotInfo slotInfo);
	UFUNCTION()
		void ChangeButtonActivation(bool bIsActivation);
	
private:
	UPROPERTY()
		class UKartInstance* GI;

	UPROPERTY()
		class UWidget_SessionInfo* SessionSlot;

};
