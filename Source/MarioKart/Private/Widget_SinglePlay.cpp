#include "Widget_SinglePlay.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UWidget_SinglePlay::NativePreConstruct()
{
}

void UWidget_SinglePlay::NativeConstruct()
{
	Button_Back2BeforeMenu->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_Back2BeforeMenu);
	Button_AccessHighlighted->OnClicked.AddDynamic(this, &UWidget_SinglePlay::OnClick_AccessHighlighted);
}

void UWidget_SinglePlay::OnClick_Back2BeforeMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Back2Menu"));
}

void UWidget_SinglePlay::OnClick_AccessHighlighted()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Access Highlighted"));
}
