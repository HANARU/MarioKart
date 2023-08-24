#include "Widget_MultiPlay.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "KartInstance.h"
#include "Widget_SessionInfo.h"
#include "Kismet/GameplayStatics.h"

void UWidget_MultiPlay::NativeConstruct()
{
	GI = GetGameInstance<UKartInstance>();

	Button_Back2BeforeMenu->OnClicked.AddDynamic(this, &UWidget_MultiPlay::OnClick_Back2BeforeMenu);
	Button_AccessHighlighted->OnClicked.AddDynamic(this, &UWidget_MultiPlay::OnClick_AccessHighlighted);
	Button_MakeLobby->OnClicked.AddDynamic(this, &UWidget_MultiPlay::OnClick_CreateLobby);
	Button_FindLobby->OnClicked.AddDynamic(this, &UWidget_MultiPlay::OnClick_FindLobby);
	Button_InputUserName->OnClicked.AddDynamic(this, &UWidget_MultiPlay::OnClick_NextInputName);

	if (MenuBGM)
	{
		BGMComp = UGameplayStatics::SpawnSound2D(GetWorld(), MenuBGM);
		if (BGMComp)
		{
			BGMComp->bIsUISound = false;
			BGMComp->bAutoDestroy = true;

			BGMComp->Play();
		}
	}

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			IMG_Fade->SetVisibility(ESlateVisibility::Hidden);
			WS_MainMenu->SetActiveWidgetIndex(ActivatedWidget);
		}
	), 1.f, false);

	if (GI != nullptr)
	{
		GI->onSearchCompleted.AddDynamic(this, &UWidget_MultiPlay::AddRoomSlot);
		GI->onFindButtonActivation.AddDynamic(this, &UWidget_MultiPlay::ChangeButtonActivation);
	}
}

void UWidget_MultiPlay::OnClick_Back2BeforeMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Back2Menu"));
	FTimerHandle DelayHandle;
	switch (ActivatedWidget)
	{
	case 0:
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				IMG_Fade->SetVisibility(ESlateVisibility::Visible);
				PlayAnimationForward(FadeIn);
				UGameplayStatics::OpenLevel(GetWorld(), LevelIntro);
			}
		), 1.f, false);
		break;
	case 1:
		WS_MainMenu->SetActiveWidgetIndex(0);
		ActivatedWidget = 0;
		break;

	default:
		break;
	}
}

void UWidget_MultiPlay::OnClick_AccessHighlighted()
{
	switch (ActivatedWidget)
	{
	case 0:
		break;
// 	case 1:
// 		if (GI != nullptr && !EText_UserName->GetText().IsEmpty())
// 		{
// 			WS_MainMenu->SetActiveWidgetIndex(1);		
// 		}
// 		break;
	case 2:
		if (GI != nullptr && !EText_RoomName->GetText().IsEmpty())
		{
			GI->CreateMySession(EText_UserName->GetText(), EText_RoomName->GetText(), 4);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("CreateSession"));
		}
		break;
	default:
		break;
	}
}

void UWidget_MultiPlay::OnClick_NextInputName()
{
    if(!EText_UserName->GetText().IsEmpty())
	{ 
	    GI->MySessionName = EText_UserName->GetText().ToString();
	    WS_MainMenu->SetActiveWidgetIndex(1);
		ActivatedWidget = 1;
	}
}

void UWidget_MultiPlay::OnClick_CreateLobby()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("CreateLobby"));
	WS_MainMenu->SetActiveWidgetIndex(2);
	ActivatedWidget = 2;

}

void UWidget_MultiPlay::OnClick_FindLobby()
{
	if (GI != nullptr)
	{
		GI->FindOtherSession();
		VB_SessionList->ClearChildren();
	}
}

void UWidget_MultiPlay::AddRoomSlot(FSessionSlotInfo slotInfo)
{
	SessionSlot = CreateWidget<UWidget_SessionInfo>(GetWorld(), sessionInfoWidget);

	if (SessionSlot != nullptr)
	{
	    //SessionSlot->text_UserName->SetText(FText::FromString(slotInfo.userName));
		SessionSlot->text_RoomName->SetText(FText::FromString(slotInfo.roomName));
		SessionSlot->text_HostName->SetText(FText::FromString(slotInfo.hostName));
		SessionSlot->text_PlayerCount->SetText(FText::FromString(slotInfo.playerCount));
		SessionSlot->text_PingSpeed->SetText(FText::AsNumber(slotInfo.pingSpeed));
		SessionSlot->sessionIndex = slotInfo.sessionIndex;
	}

	VB_SessionList->AddChild(SessionSlot);
}

void UWidget_MultiPlay::ChangeButtonActivation(bool bIsActivation)
{
	Button_FindLobby->SetIsEnabled(bIsActivation);
}


