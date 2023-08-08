#include "Widget_Lobby.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"

void UWidget_Lobby::NativeConstruct()
{
	Button_Check->OnClicked.AddDynamic(this, &UWidget_Lobby::ClickedCheck);

}

void UWidget_Lobby::ClickedSinglePlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Check"));
}

void UWidget_Lobby::ClickedCheck()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Check"));
}


