#include "Widget_SinglePlay.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Components/Image.h"
#include "OverlayActor.h"
#include "Kismet/GameplayStatics.h"

#define SelectGame 0
#define ChoosePlayer 1
#define NoticeMap 2


void UWidget_SinglePlay::NativeConstruct()
{
	Button_Back2BeforeMenu->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_Back2BeforeMenu);
	Button_AccessHighlighted->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_AccessHighlighted);
	Button_VS_Race->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_Press_VS_Race);
	Button_Mario->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_Press_Mario);
	Button_MushRoom->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_Press_MushRoomMap);

	PlayAnimationForward(FadeOut);
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			IMG_Fade->SetVisibility(ESlateVisibility::Hidden);
			WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		}
	), 1.f, false);
	
	OverlayActor = Cast<AOverlayActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverlayActor::StaticClass()));

}

void UWidget_SinglePlay::OnClick_Back2BeforeMenu()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Back2Menu"));
	FTimerHandle DelayHandle;
	switch (ActivatedWidget)
	{
	case SelectGame:
		
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				IMG_Fade->SetVisibility(ESlateVisibility::Visible);
				PlayAnimationForward(FadeIn);
				UGameplayStatics::OpenLevel(GetWorld(), LevelIntro);
			}
		), 1.f, false);

	case ChoosePlayer:		
		ActivatedWidget = SelectGame;
		WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		return;
	case NoticeMap:		
		ActivatedWidget = ChoosePlayer;
		WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		return;
	default:
		break;
	}
}

void UWidget_SinglePlay::OnClick_AccessHighlighted()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Access Highlighted"));
	switch (ActivatedWidget)
	{
	case SelectGame:
		ActivatedWidget = ChoosePlayer;
		WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		return;
	case ChoosePlayer:
		ActivatedWidget = NoticeMap;
		WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		return;
	default:
		break;
	}
}

void UWidget_SinglePlay::OnClick_Press_VS_Race()
{
	ActivatedWidget = 1;
	WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
}

void UWidget_SinglePlay::OnClick_Press_Mario()
{
	ActivatedWidget = 2;
	WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
}

void UWidget_SinglePlay::OnHover_Mario()
{
	OverlayActor->KartNumber = 0;
	OverlayActor->PlayerNumber = 0;
}

void UWidget_SinglePlay::OnClick_Press_MushRoomMap()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Game Ready!!"));
}

void UWidget_SinglePlay::OnClick_Press_Luige()
{
	OverlayActor->KartNumber = 0;
	OverlayActor->PlayerNumber = 1;
}

void UWidget_SinglePlay::OnHover_Luige()
{
	ActivatedWidget = 2;
	WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
}
