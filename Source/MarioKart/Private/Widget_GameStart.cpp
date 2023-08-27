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
    RenderArray.SetNum(4);

    IMG_MarioReady->SetRenderOpacity(0);
    IMG_LuigeReady->SetRenderOpacity(0);
    IMG_DaisyReady->SetRenderOpacity(0);
    IMG_YoshiReady->SetRenderOpacity(0);

    GameKartInstance = Cast<UKartInstance>(GetGameInstance());
}

void UWidget_GameStart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 세션에 들어온 모든 플레이어의 정보 출력
    TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;

    UE_LOG(LogTemp, Warning, TEXT("Player Num : %d"), players.Num());

    for (int index = 0; index < players.Num(); ++index)
    {
        AState_KartPlayer* KartPlayerState = Cast<AState_KartPlayer>(players[index]);
        
        if (KartPlayerState != nullptr)
        {
            PlayerArray[index] = FText::FromString(*KartPlayerState->GetPlayerName());
            RenderArray[index] = 1;
            KartPlayerState->PlayerNum = index;
            GameKartInstance->CurrentPlayerNum = index;
        }
        UE_LOG(LogTemp, Warning, TEXT("Player : %s, %d"), KartPlayerState != nullptr ? *KartPlayerState->GetPlayerName() : *FString("Null"), KartPlayerState->PlayerNum);
    }
    text_player_1->SetText(PlayerArray[0]);
    text_player_2->SetText(PlayerArray[1]);
    text_player_3->SetText(PlayerArray[2]);
    text_player_4->SetText(PlayerArray[3]);

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
                //UGameplayStatics::OpenLevel(this, LevelMultiStardium);
                //GetWorld()->ServerTravel("/Game/1_Level/7_Multi_Stardium?listen");
                GetWorld()->ServerTravel("/Game/1_Level/10_Multi_Stardium?listen");
            }
        ), 2, false);
    }
}