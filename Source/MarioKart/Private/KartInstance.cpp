#include "KartInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UKartInstance::UKartInstance()
{
	MySessionName = TEXT("KartPlayers");
}

void UKartInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* subsys = IOnlineSubsystem::Get())
	{
		// Online Session Interface API ���ٿ� �ν��Ͻ� ��������
		sessionInterface = subsys->GetSessionInterface();

		// ���� �̺�Ʈ�� �Լ� ���ε��ϱ�
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UKartInstance::OnCreatedMySession);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UKartInstance::OnFindOtherSessions);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UKartInstance::OnJoinSelectedSession);
	}
}
///////////////////////////////////////////////////
//void UKartInstance::Host()
//{
//	UEngine* Engine = GetEngine(); //���ӿ����� ���� �ν��Ͻ��� ������
//
//	if (!ensure(Engine != nullptr)) return;
//
//	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting")); //�α׶����
//
//	UWorld* World = GetWorld(); // ������ ���� ������ ������
//
//	if (!ensure(World != nullptr)) return;
//	//�������� .  ������ �Է��� ����� ������ �̵���Ŵ, Ŭ���̾�Ʈ�� ���̰� ������ �������� Ŭ���̾�Ʈ����
//	//�÷��̾� ��Ʈ�ѷ����� ClientTravel�� ȣ����.
//	World->ServerTravel("/Game/1_Level/Level_Stardium.Level_Stardium");
//}
//
//void UKartInstance::Join(const FString& Address)
//{
//	UEngine* Engine = GetEngine();
//
//	//���� Ŭ���� �÷��̾���Ʈ�ѷ��� ù��° �÷��̾� ��Ʈ�ѷ� ������	
//	APlayerController* PlayerController = GetFirstLocalPlayerController();
//
//	if (!ensure(Engine != nullptr)) return;
//	//�α�
//	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
//
//	//�����ǰ� ��ȿ�Ҷ� ȣ��Ǹ� �Էµ� �������ּ��� ������ �̵���.  
//	//���ڸ����� ������ �����ϼ�.
//	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
//}

void UKartInstance::CreateMySession(FText userName, FText roomName, int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;

	// 1. LAN �������� DEDICATED �������� �����Ѵ�.
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. �˻� ������ ������ �����Ѵ�.
	sessionSettings.bShouldAdvertise = true;

	// 3. �ڱ� ������ ���޵� �� �ְ� �����Ѵ�.
	sessionSettings.bUsesPresence = true;

	// 4. �ٸ� ������ �߰� ������ ����Ѵ�.
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// 5. ���� ���� �ο��� �����Ѵ�.
	sessionSettings.NumPublicConnections = 4;

	// 6. ���ǿ� �߰� ������ �ִ´�.
	sessionSettings.Set(FName("ROOM_NAME"), roomName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("USER_NAME"), userName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);//�߰�

	bool isSuccess = sessionInterface->CreateSession(0, FName(roomName.ToString()), sessionSettings);
	UE_LOG(LogTemp, Warning, TEXT("Session Create Result: %s"), isSuccess ? *FString("Success") : *FString("Failed..."));
}

void UKartInstance::OnCreatedMySession(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		bool result = GetWorld()->ServerTravel("/Game/1_Level/6_Lobby_Multi_Ready?Listen", true);
		UE_LOG(LogTemp, Warning, TEXT("Travel Result: %s"), result ? *FString("Success") : *FString("Failed..."));
	}
}
// /Game/1_Level/6_Lobby_Multi_Ready
// /Game/1_Level/7_Multi_Stardium
void UKartInstance::FindOtherSession()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. ���� �˻��� LAN���� �� ������ ���θ� �����Ѵ�.
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. ���� ����(query) �����Ѵ�.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 3. ������ �˻����� �����Ѵ�.
	sessionSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	onFindButtonActivation.Broadcast(false);
}

void UKartInstance::OnFindOtherSessions(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// �˻��� ���� ����� �����´�.
		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("Find Sessions: %d"), searchResults.Num());

		for (int32 i = 0; i < searchResults.Num(); i++)
		{
			FString userName;
			searchResults[i].Session.SessionSettings.Get(FName("USER_NAME"), userName);//�߰�
			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			FString hostName;
			searchResults[i].Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
			int32 openNumber = searchResults[i].Session.NumOpenPublicConnections;
			int32 maxNumber = searchResults[i].Session.SessionSettings.NumPublicConnections;
			int32 pingSpeed = searchResults[i].PingInMs;

			UE_LOG(LogTemp, Warning, TEXT("User Name: %s, Room Name: %s, HostName: %s, OpenNumber: %d, MaxNumber: %d, Ping Speed: %d"),*userName, *roomName, *hostName, openNumber, maxNumber, pingSpeed);

			// ����ü ������ ã�� ���� ������ �Է��Ѵ�.
			FSessionSlotInfo slotInfo;
			slotInfo.Set(userName, roomName, hostName, FString::Printf(TEXT("(%d/%d)"), maxNumber - openNumber, maxNumber), pingSpeed, i);

			// ���� ������ ��������Ʈ�� �����Ѵ�.
			onSearchCompleted.Broadcast(slotInfo);
		}

		onFindButtonActivation.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search failed..."));
	}
}

void UKartInstance::JoinSelectedSession(int32 index)
{
	bool isJoin = sessionInterface->JoinSession(0, FName(MySessionName), sessionSearch->SearchResults[index]);
	UE_LOG(LogTemp, Warning, TEXT("Session Name : %s, UserName : %s"), *MySessionName, *sessionSearch->SearchResults[index].Session.OwningUserName);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Join %s"), isJoin ? *FString("Success") : *FString("Failed..")));
	//GetWorld()->GetFirstPlayerController()->ClientTravel("192.168.0.40:7777", ETravelType::TRAVEL_Absolute);
}

void UKartInstance::OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("Success JoinSession"));

	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), result == EOnJoinSessionCompleteResult::Success ? *FString::Printf(TEXT("Success: %s"), *sessionName.ToString()) : *FString(TEXT("Failed")));

		APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

		if (playerCon != nullptr)
		{
			// join�� ���� ȣ��Ʈ�� ServerTravel �� �� �ּҸ� �޾ƿ´�.
			FString url;
			sessionInterface->GetResolvedConnectString(sessionName, url);
			UE_LOG(LogTemp, Warning, TEXT("Connection URL: %s"), *url);

			// �ּҸ� �޾Ҵٸ�, �� �ּҿ� ���� �� �̵��Ѵ�.
			if (!url.IsEmpty())
			{
				playerCon->ClientTravel(url, ETravelType::TRAVEL_Absolute);
			}
		}

	}
	break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("SessionIsFull"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("SessionDoesNotExist"));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("CouldNotRetrieveAddress"));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("AlreadyInSession"));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("UnknownError"));
		break;
	default:
		break;
	}
}
