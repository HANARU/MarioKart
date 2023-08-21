#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KartInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FString roomName = TEXT("");
	FString hostName = TEXT("");
	FString playerCount = TEXT("");
	int32 pingSpeed = 0;
	int32 sessionIndex = 0;

	FORCEINLINE void Set(FString rName, FString hName, FString pCount, int32 ping, int32 index) { roomName = rName; hostName = hName; playerCount = pCount; pingSpeed = ping; sessionIndex = index; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, FSessionSlotInfo, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonSignature, bool, bIsActivation);


UCLASS()
class MARIOKART_API UKartInstance : public UGameInstance
{
	GENERATED_BODY()
		

public:
	UKartInstance();

	virtual void Init() override;

	//////////////////////////////////////////////////
	//UFUNCTION(Exec)
	//void Host();

	//UFUNCTION(Exec)
	//void Join(const FString& Address);

	UPROPERTY()
		FString MySessionName;

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FSearchSignature onSearchCompleted;
	FFindButtonSignature onFindButtonActivation;

	void CreateMySession(FText roomName, int32 playerCount);
	void OnCreatedMySession(FName sessionName, bool bWasSuccessful);
	void FindOtherSession();
	void OnFindOtherSessions(bool bWasSuccessful);
	void JoinSelectedSession(int32 index);
	void OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);

};
