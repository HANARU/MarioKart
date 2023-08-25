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
		// Online Session Interface API 접근용 인스턴스 가져오기
		sessionInterface = subsys->GetSessionInterface();

		// 세션 이벤트에 함수 바인딩하기
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UKartInstance::OnCreatedMySession);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UKartInstance::OnFindOtherSessions);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UKartInstance::OnJoinSelectedSession);
	}
}
///////////////////////////////////////////////////
//void UKartInstance::Host()
//{
//	UEngine* Engine = GetEngine(); //게임엔진의 현재 인스턴스를 가져움
//
//	if (!ensure(Engine != nullptr)) return;
//
//	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting")); //로그띄워줌
//
//	UWorld* World = GetWorld(); // 레벨의 현재 레벨을 가져옴
//
//	if (!ensure(World != nullptr)) return;
//	//서버전용 .  서버를 입력한 경로의 맵으로 이동시킴, 클라이언트랑 같이감 서버가 접속중인 클라이언트들의
//	//플레이어 컨트롤러에서 ClientTravel을 호출함.
//	World->ServerTravel("/Game/1_Level/Level_Stardium.Level_Stardium");
//}
//
//void UKartInstance::Join(const FString& Address)
//{
//	UEngine* Engine = GetEngine();
//
//	//현재 클라의 플레이어컨트롤러의 첫번째 플레이어 컨트롤러 가져옴	
//	APlayerController* PlayerController = GetFirstLocalPlayerController();
//
//	if (!ensure(Engine != nullptr)) return;
//	//로그
//	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
//
//	//아이피가 유효할때 호출되면 입력된 아이피주소의 서버로 이동함.  
//	//인텔리센스 빨간줄 무시하셈.
//	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
//}

void UKartInstance::CreateMySession(FText userName, FText roomName, int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;

	// 1. LAN 연결인지 DEDICATED 연결인지 설정한다.
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. 검색 가능한 방으로 설정한다.
	sessionSettings.bShouldAdvertise = true;

	// 3. 자기 정보를 전달될 수 있게 설정한다.
	sessionSettings.bUsesPresence = true;

	// 4. 다른 유저의 중간 입장을 허용한다.
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// 5. 입장 가능 인원을 설정한다.
	sessionSettings.NumPublicConnections = 4;

	// 6. 세션에 추가 설정을 넣는다.
	sessionSettings.Set(FName("ROOM_NAME"), roomName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("HOST_NAME"), MySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("USER_NAME"), userName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);//추가

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

	// 1. 세션 검색을 LAN으로 할 것인지 여부를 설정한다.
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 2. 세션 쿼리(query) 설정한다.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 3. 세션의 검색량을 설정한다.
	sessionSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	onFindButtonActivation.Broadcast(false);
}

void UKartInstance::OnFindOtherSessions(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		// 검색된 세션 목록을 가져온다.
		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("Find Sessions: %d"), searchResults.Num());

		for (int32 i = 0; i < searchResults.Num(); i++)
		{
			FString userName;
			searchResults[i].Session.SessionSettings.Get(FName("USER_NAME"), userName);//추가
			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			FString hostName;
			searchResults[i].Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
			int32 openNumber = searchResults[i].Session.NumOpenPublicConnections;
			int32 maxNumber = searchResults[i].Session.SessionSettings.NumPublicConnections;
			int32 pingSpeed = searchResults[i].PingInMs;

			UE_LOG(LogTemp, Warning, TEXT("User Name: %s, Room Name: %s, HostName: %s, OpenNumber: %d, MaxNumber: %d, Ping Speed: %d"),*userName, *roomName, *hostName, openNumber, maxNumber, pingSpeed);

			// 구조체 변수에 찾은 세션 정보를 입력한다.
			FSessionSlotInfo slotInfo;
			slotInfo.Set(userName, roomName, hostName, FString::Printf(TEXT("(%d/%d)"), maxNumber - openNumber, maxNumber), pingSpeed, i);

			// 세션 정보를 델리게이트로 전파한다.
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
			// join된 세션 호스트의 ServerTravel 된 맵 주소를 받아온다.
			FString url;
			sessionInterface->GetResolvedConnectString(sessionName, url);
			UE_LOG(LogTemp, Warning, TEXT("Connection URL: %s"), *url);

			// 주소를 받았다면, 그 주소에 따라서 맵 이동한다.
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
