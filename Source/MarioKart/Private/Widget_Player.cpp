#include "Widget_Player.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Components/TextBlock.h"

void UWidget_Player::NativePreConstruct()
{

}

void UWidget_Player::NativeConstruct()
{

}

void UWidget_Player::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    // 세션에 들어온 모든 플레이어의 정보 출력
    TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
    FString playersInfoText;

    for (APlayerState* p : players)
    {
        playersInfoText.Append(FString::Printf(TEXT("%s\n"), *p->GetPlayerName()));
    }

    text_players->SetText(FText::FromString(playersInfoText));
 
 }
