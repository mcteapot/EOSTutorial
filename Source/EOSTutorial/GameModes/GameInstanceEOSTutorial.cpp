// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceEOSTutorial.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

void UGameInstanceEOSTutorial::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineIdentityPtr IdentityPtr = OnlineSubystem->GetIdentityInterface();
		if (IdentityPtr.IsValid())
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;
			
			IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UGameInstanceEOSTutorial::LoginWithEOSReturn);
			IdentityPtr->Login(0, AccountDetails);
		}
	}
}

FString UGameInstanceEOSTutorial::GetPlayerUsername()
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineIdentityPtr IdentityPtr = OnlineSubystem->GetIdentityInterface();
		if (IdentityPtr.IsValid())
		{
			FString username = IdentityPtr->GetPlayerNickname(0);
			return username;
		}
	}
	return FString(); // returns empty string if no username is not logged in
}

bool UGameInstanceEOSTutorial::IsPlayerLoggedIn()
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineIdentityPtr IdentityPtr = OnlineSubystem->GetIdentityInterface();
		if (IdentityPtr.IsValid())
		{
			return IdentityPtr->GetLoginStatus(0) == ELoginStatus::LoggedIn;
		}
	}
	return false; // returns false if no username is not logged in
}

void UGameInstanceEOSTutorial::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer,
	int32 NumberOfPuglicConnections)
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubystem->GetSessionInterface();
		if(SessionPtr.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			//Setting properteis of sessions struct
			SessionSettings.bIsDedicated = bIsDedicatedServer;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsLANMatch = bIsLanServer;
			SessionSettings.NumPublicConnections = NumberOfPuglicConnections;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bUseLobbiesIfAvailable = true;
			SessionSettings.Set(SEARCH_KEYWORDS, EOSSessionKeyword, EOnlineDataAdvertisementType::ViaOnlineService);
			
			//Creating session
			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnCreateSessionComplete);
			SessionPtr->CreateSession(0, EOSSessionName, SessionSettings);
			//SessionPtr->CreateSession(0, FName("MainSession"), SessionCreationInfo);
		}
		
	}
}

void UGameInstanceEOSTutorial::FindSessionAndJoin()
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubystem->GetSessionInterface();
		if(SessionPtr.IsValid())
		{
			//Searching for lobbies, flase for nomral match making session 
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->MaxSearchResults = 5000;
			SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, EOSSessionKeyword, EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			//SessionSearch->QuerySettings.SearchParams.Empty();

			// Setting the delegate for when the search is complete and calling the search function to find session
			SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnFindSessionComplete);
			SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UGameInstanceEOSTutorial::JoinSession()
{
}

void UGameInstanceEOSTutorial::DestorySession()
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		IOnlineSessionPtr SessionPtr = OnlineSubystem->GetSessionInterface();
		if(SessionPtr.IsValid())
		{
			SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnDestroySessionComplete);
			SessionPtr->DestroySession(EOSSessionName);
		}
	}
}


void UGameInstanceEOSTutorial::LoginWithEOSReturn(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                                  const FString& Error)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Successful"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed - %S"), *Error);
	}
}

void UGameInstanceEOSTutorial::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		GetWorld()->ServerTravel(OpenLevelLocationText);
		UE_LOG(LogTemp, Warning, TEXT("Session Sreatedd And Server Travel"));
	}
}

void UGameInstanceEOSTutorial::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Destroyed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Destory Failed"));
	}
}

void UGameInstanceEOSTutorial::OnFindSessionComplete(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking For Session"));
		// If session is found then join it
		IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
		if (OnlineSubystem)
		{
			IOnlineSessionPtr SessionPtr = OnlineSubystem->GetSessionInterface();
			if(SessionPtr.IsValid())
			{
				// Join first session that is found
				UE_LOG(LogTemp, Warning, TEXT("Sessions Found - %d"), SessionSearch->SearchResults.Num());
				if(SessionSearch->SearchResults.Num())
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Session"));
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnJoinSessionComplete);
					SessionPtr->JoinSession(0, EOSSessionName, SessionSearch->SearchResults[0]);
					
					if(SessionSearch->SearchResults[0].IsValid())
					{
						/*
						SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnJoinSessionComplete);
						SessionPtr->JoinSession(0, EOSSessionName, SessionSearch->SearchResults[0]);
						*/
						SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnJoinSessionComplete);
						SessionPtr->JoinSession(0, EOSSessionName, SessionSearch->SearchResults[0]);
					}
					

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Session Found"));
					// IF no session is found then create one
					CreateEOSSession(false, false, 10);
				}
				//Cears the session find delegate
				SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Failed"));
		// IF no session is found then create one
		CreateEOSSession(false, false, 10);
	}
}

void UGameInstanceEOSTutorial::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem *OnlineSubystem = Online::GetSubsystem(this->GetWorld());
	if (OnlineSubystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName, ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
	
}
