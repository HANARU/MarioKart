// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_GameStart.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"
#include "State_KartPlayer.h"

void UWidget_GameStart::NativePreConstruct()
{
    Super::NativePreConstruct();
}

void UWidget_GameStart::NativeConstruct()
{
    btn_GameStart->OnClicked.AddDynamic(this, &UWidget_GameStart::OnClickCreateButton);

    PlayAnimationForward(TrophyBlankFade);
}

void UWidget_GameStart::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 세션에 들어온 모든 플레이어의 정보 출력
    TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
    FString playersInfoText;
    UE_LOG(LogTemp, Warning, TEXT("Player Num : %d"), players.Num());
    for (APlayerState* p : players)
    {
        
        AState_KartPlayer* KartPlayerState = Cast<AState_KartPlayer>(p);

        UE_LOG(LogTemp, Warning, TEXT("Player : %s"), KartPlayerState != nullptr ?  *KartPlayerState->GetPlayerName() : *FString("Null"));
        if (KartPlayerState != nullptr)
        {
            playersInfoText.Append(FString::Printf(TEXT("%s, "), *KartPlayerState->GetPlayerName()));
        }
    }

    text_players->SetText(FText::FromString(playersInfoText));
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
