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
