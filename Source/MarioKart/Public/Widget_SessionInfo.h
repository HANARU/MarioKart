// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_SessionInfo.generated.h"

/**
 * 
 */
UCLASS()
class MARIOKART_API UWidget_SessionInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = Menu)
		class UButton* Button_SessionInfo;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
		class UTextBlock* text_HostName;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
		class UTextBlock* text_PlayerCount;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
		class UTextBlock* text_PingSpeed;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
		class UTextBlock* text_RoomName;

	int32 sessionIndex = 0;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UKartInstance* GI;
	
	UFUNCTION()
		void OnClickedThisButton();
};
