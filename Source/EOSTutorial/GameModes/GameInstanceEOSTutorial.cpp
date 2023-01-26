// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceEOSTutorial.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

void UGameInstanceEOSTutorial::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineIdentityPtr identityInterface = subsystemRef->GetIdentityInterface();
		if (identityInterface.IsValid())
		{
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Id = ID;
			AccountDetails.Token = Token;
			AccountDetails.Type = LoginType;
			
			identityInterface->OnLoginCompleteDelegates->AddUObject(this, &UGameInstanceEOSTutorial::LoginWithEOSReturn);
			identityInterface->Login(0, AccountDetails);
		}
	}
}

FString UGameInstanceEOSTutorial::GetPlayerUsername()
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineIdentityPtr identityInterface = subsystemRef->GetIdentityInterface();
		if (identityInterface.IsValid())
		{
			FString username = identityInterface->GetPlayerNickname(0);
			return username;
		}
	}
	return FString(); // returns empty string if no username is not logged in
}

bool UGameInstanceEOSTutorial::IsPlayerLoggedIn()
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineIdentityPtr identityInterface = subsystemRef->GetIdentityInterface();
		if (identityInterface.IsValid())
		{
			return identityInterface->GetLoginStatus(0) == ELoginStatus::LoggedIn;
		}
	}
	return false; // returns false if no username is not logged in
}

void UGameInstanceEOSTutorial::CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer,
	int32 NumberOfPuglicConnections)
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = subsystemRef->GetSessionInterface();
		if(SessionPtrRef.IsValid())
		{
			FOnlineSessionSettings SessionCreationInfo;
			//Setting properteis of sessions struct
			SessionCreationInfo.bIsDedicated = bIsDedicatedServer;
			SessionCreationInfo.bAllowInvites = true;
			SessionCreationInfo.bIsLANMatch = bIsLanServer; // When set to true will not create online session
			SessionCreationInfo.NumPublicConnections = NumberOfPuglicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = true; 
			SessionCreationInfo.bUsesPresence = false; 
			SessionCreationInfo.bShouldAdvertise = true;
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("RandomHi"), EOnlineDataAdvertisementType::ViaOnlineService);

			//Creating session
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnCreateSessionComplete);
			SessionPtrRef->CreateSession(0, EOSSessionName, SessionCreationInfo);
			//SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
		}
		
	}
}

void UGameInstanceEOSTutorial::FindSessionAndJoin()
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = subsystemRef->GetSessionInterface();
		if(SessionPtrRef.IsValid())
		{
			//Searching for lobbies, flase for nomral match making session 
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			//SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, EOSSessionName.ToString(), EOnlineComparisonOp::Equals);
			SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, false, EOnlineComparisonOp::Equals); // Was set to false 
			SessionSearch->bIsLanQuery = false;
			SessionSearch->MaxSearchResults = 20;
			//SessionSearch->QuerySettings.SearchParams.Empty();

			// Setting the delegate for when the search is complete and calling the search function to find session
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnFindSessionComplete);
			SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UGameInstanceEOSTutorial::JoinSession()
{
}

void UGameInstanceEOSTutorial::DestorySession()
{
	IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
	if (subsystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = subsystemRef->GetSessionInterface();
		if(SessionPtrRef.IsValid())
		{
			SessionPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnDestroySessionComplete);
			SessionPtrRef->DestroySession(EOSSessionName);
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
		IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
		if (subsystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = subsystemRef->GetSessionInterface();
			if(SessionPtrRef.IsValid())
			{
				// Join first session that is found
				UE_LOG(LogTemp, Warning, TEXT("Sessions Found - %d"), SessionSearch->SearchResults.Num());
				if(SessionSearch->SearchResults.Num() > 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Session"));
					SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnJoinSessionComplete);
					SessionPtrRef->JoinSession(0, EOSSessionName, SessionSearch->SearchResults[0]);
					/*
					if(SessionSearch->SearchResults[0].IsValid())
					{
						SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstanceEOSTutorial::OnJoinSessionComplete);
						SessionPtrRef->JoinSession(0, EOSSessionName, SessionSearch->SearchResults[0]);
					}
					*/
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No Session Found"));
					// IF no session is found then create one
					CreateEOSSession(false, false, 10);
				}
				//Cears the session find delegate
				SessionPtrRef->ClearOnFindSessionsCompleteDelegates(this);
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

	// What happens when the session is joined
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		// Get the player controller
		APlayerController *PlayerControllerRef  = UGameplayStatics::GetPlayerController(GetWorld(),0);
		if(PlayerControllerRef)
		{
			FString JoinAddress;
			IOnlineSubsystem *subsystemRef = Online::GetSubsystem(this->GetWorld());
			if (subsystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = subsystemRef->GetSessionInterface();
				if(SessionPtrRef.IsValid())
				{
					// Get the resolved connect string
					SessionPtrRef->GetResolvedConnectString(SessionName, JoinAddress);
					UE_LOG(LogTemp, Warning, TEXT("Join Address is %S"), *JoinAddress)
					if(!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(SessionName.ToString(), ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Join Failed"));
	}
	
}
