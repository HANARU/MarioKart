#include "Widget_Lobby.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UWidget_Lobby::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWidget_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	Button_PressStartAtIntro->OnClicked.AddDynamic(this, &UWidget_Lobby::OnClickedPressIntro2Menu);
	Button_PressSinglePlay->OnClicked.AddDynamic(this, &UWidget_Lobby::OnClickedPressMenu2Single);
	Button_PressMultiPlay->OnClicked.AddDynamic(this, &UWidget_Lobby::OnClickedPressMenu2Multi);

	PlayAnimationForward(IntroInit);

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			IMG_InitFade->SetVisibility(ESlateVisibility::Hidden);
		}
	), 1, false);
	
}

void UWidget_Lobby::OnClickedPressIntro2Menu()
{
	if (Intro2Menu != nullptr)
	{
		PlayAnimationForward(Intro2Menu);

		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				WS_Intro->SetActiveWidgetIndex(1);
				PlayAnimationForward(MainMenuStart);
			}
		), 1.5f, false);
	}
}

void UWidget_Lobby::OnClickedPressMenu2Single()
{
	if (Menu2RaceType != nullptr)
	{
		IMG_NextMenu->SetVisibility(ESlateVisibility::Visible);
		PlayAnimationForward(Menu2RaceType);
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				UGameplayStatics::OpenLevel(this, LevelSingle);
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Single"));
			}
		), 4, false);
	}
}

void UWidget_Lobby::OnClickedPressMenu2Multi()
{
	if (Menu2RaceType != nullptr)
	{
		IMG_NextMenu->SetVisibility(ESlateVisibility::Visible);
		PlayAnimationForward(Menu2RaceType);
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				UGameplayStatics::OpenLevel(this, LevelMulti);
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Multi"));
			}
		), 4, false);
	}
}
