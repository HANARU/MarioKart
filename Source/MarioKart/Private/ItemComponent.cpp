// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "KartPlayer.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GM_Race.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GameMode = Cast<AGM_Race>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		GameMode->ItemSignature.BindUObject(this, &UItemComponent::ReceiveItemfromCharacter);
	}

}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Item1);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, Item2);

}

void UItemComponent::ReceiveItemfromCharacter(int32 ItemNum)
{
	if (Current1stItem == 12 && Current2ndItem == 12)
	{
		Current1stItem = ItemNum;
	}
	else if (Current1stItem != 12 && Current2ndItem == 12)
	{
		Current2ndItem = Current1stItem;
		Current1stItem = ItemNum;
	}
	Item1 = UKismetStringLibrary::Conv_IntToString(Current1stItem);
	Item2 = UKismetStringLibrary::Conv_IntToString(Current2ndItem);
	
}

void UItemComponent::UsingItem()
{
	if (Current2ndItem == 12)
	{
		Current1stItem = 12;
	}
	if (Current2ndItem != 12)
	{
		Current1stItem = Current2ndItem;
		Current2ndItem = 12;
	}

	Item1 = UKismetStringLibrary::Conv_IntToString(Current1stItem);
	Item2 = UKismetStringLibrary::Conv_IntToString(Current2ndItem);
}

