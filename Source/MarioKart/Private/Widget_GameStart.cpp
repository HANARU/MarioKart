// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_GameStart.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "State_KartPlayer.h"
#include "GameFramework/PlayerState.h"

void UWidget_GameStart::NativePreConstruct()
{
    Super::NativePreConstruct();
}

void UWidget_GameStart::NativeConstruct()
{
    btn_GameStart->OnClicked.AddDynamic(this, &UWidget_GameStart::OnClickCreateButton);

    PlayAnimationForward(TrophyBlankFade);

    PlayerArray.SetNum(4);
    TextureArray.SetNum(4);
    RenderArray.SetNum(4);

    //IMG_MarioReady->SetVisibility(ESlateVisibility::Hidden);
    //IMG_LuigeReady->SetVisibility(ESlateVisibility::Hidden);
    //IMG_DaisyReady->SetVisibility(ESlateVisibility::Hidden);
    //IMG_YoshiReady->SetVisibility(ESlateVisibility::Hidden);

    IMG_MarioReady->SetRenderOpacity(0);
    IMG_LuigeReady->SetRenderOpacity(0);
    IMG_DaisyReady->SetRenderOpacity(0);
    IMG_YoshiReady->SetRenderOpacity(0);
}

void UWidget_GameStart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 세션에 들어온 모든 플레이어의 정보 출력
    TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
    FString playersInfoText;
    UE_LOG(LogTemp, Warning, TEXT("Player Num : %d"), players.Num());

    for (int index = 0; index < players.Num(); ++index)
    {
        AState_KartPlayer* KartPlayerState = Cast<AState_KartPlayer>(players[index]);
        UE_LOG(LogTemp, Warning, TEXT("Player : %s"), KartPlayerState != nullptr ? *KartPlayerState->GetPlayerName() : *FString("Null"));
        if (KartPlayerState != nullptr)
        {
            PlayerArray[index] = FText::FromString(*KartPlayerState->GetPlayerName());
            TextureArray[index] = ESlateVisibility::Visible;
            RenderArray[index] = 1;
        }
    }
    text_player_1->SetText(PlayerArray[0]);
    text_player_2->SetText(PlayerArray[1]);
    text_player_3->SetText(PlayerArray[2]);
    text_player_4->SetText(PlayerArray[3]);

    //IMG_MarioReady->SetVisibility(TextureArray[0]);
    //IMG_LuigeReady->SetVisibility(TextureArray[1]);
    //IMG_DaisyReady->SetVisibility(TextureArray[2]);
    //IMG_YoshiReady->SetVisibility(TextureArray[3]);

    IMG_MarioReady->SetRenderOpacity(RenderArray[0]);
    IMG_LuigeReady->SetRenderOpacity(RenderArray[1]);
    IMG_DaisyReady->SetRenderOpacity(RenderArray[2]);
    IMG_YoshiReady->SetRenderOpacity(RenderArray[3]);

    UE_LOG(LogTemp, Warning, TEXT("%f"), RenderArray[0]);
}

void UWidget_GameStart::OnClickCreateButton()
{
    if (TrophyBlankFade != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Button Clicked by a client."));
        FTimerHandle DelayHandle;
        GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
            {
                UGameplayStatics::OpenLevel(this, LevelMultiStardium);
            }
        ), 2, false);
    }
}