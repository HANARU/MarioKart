#include "ItemComponent.h"
#include "KartPlayer.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GM_Race.h"

#define NoItem 12

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->ItemSignature.BindUObject(this, &UItemComponent::ReceiveItemfromCharacter);
	}
	Item1 = UKismetStringLibrary::Conv_IntToString(Current1stItem);
	Item2 = UKismetStringLibrary::Conv_IntToString(Current2ndItem);

}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Item1);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, Item2);

}

void UItemComponent::ReceiveItemfromCharacter(int32 ItemNum)
{
	if (Current1stItem == NoItem && Current2ndItem == NoItem)
	{
		Current1stItem = ItemNum;
	}
	else if (Current1stItem != NoItem && Current2ndItem == NoItem)
	{
		Current2ndItem = Current1stItem;
		Current1stItem = ItemNum;
	}

	
}

void UItemComponent::UsingItem()
{
	if (Current2ndItem == NoItem)
	{
		Current1stItem = NoItem;
	}
	if (Current2ndItem != NoItem)
	{
		Current1stItem = Current2ndItem;
		Current2ndItem = NoItem;
	}
}

void UItemComponent::OnRep_CurrentItemData()
{

}

void UItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AKartPlayer, this);
}