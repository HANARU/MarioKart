// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_GameStart.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

void UWidget_GameStart::NativePreConstruct()
{
    Super::NativePreConstruct();
}

void UWidget_GameStart::NativeConstruct()
{
    btn_GameStart->OnClicked.AddDynamic(this, &UWidget_GameStart::OnClickCreateButton);

    PlayAnimationForward(TrophyBlankFade);
}

void UWidget_GameStart::OnClickCreateButton()
{
    if (TrophyBlankFade != nullptr)
    {
        FTimerHandle DelayHandle;
        GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
            {
                UGameplayStatics::OpenLevel(this, LevelMultiStardium);
            }
        ), 2, false);
    }
}
