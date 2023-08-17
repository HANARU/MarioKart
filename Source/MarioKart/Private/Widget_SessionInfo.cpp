#include "Widget_SessionInfo.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "KartInstance.h"
#include "Kismet/GameplayStatics.h"

void UWidget_SessionInfo::NativeConstruct()
{
	GI = GetGameInstance<UKartInstance>();

	Button_SessionInfo->OnClicked.AddDynamic(this, &UWidget_SessionInfo::OnClickedThisButton);
}

void UWidget_SessionInfo::OnClickedThisButton()
{
	if (GI != nullptr)
	{
		GI->JoinSelectedSession(sessionIndex);
	}
}
