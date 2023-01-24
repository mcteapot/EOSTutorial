// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceEOSTutorial.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"

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
}

void UGameInstanceEOSTutorial::FindSessionAndJoin()
{
}

void UGameInstanceEOSTutorial::JoinSession()
{
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
}

void UGameInstanceEOSTutorial::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UGameInstanceEOSTutorial::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}
