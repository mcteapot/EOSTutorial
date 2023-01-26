// Fill out your copyright notice in the Description page of Project Settings.

#include "EOSGameModeSessions.h"

#include "GameInstanceEOSTutorial.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"




AEOSGameModeSessions::AEOSGameModeSessions()
{
}

void AEOSGameModeSessions::PostLogin(APlayerController* NewPlayer)
{
	// Call to base class for default functionality
	Super::PostLogin(NewPlayer);


	if(NewPlayer)
	{
		FUniqueNetIdRepl UniquieNetIdRepl;

		if(NewPlayer->IsLocalController())
		{
			// Checks to see if the player is host of the session
			ULocalPlayer *LocalPlayerRef = NewPlayer->GetLocalPlayer();
			if(LocalPlayerRef)
			{
				// Player is host, so we can get the unique net id
				UniquieNetIdRepl = LocalPlayerRef->GetPreferredUniqueNetId();
			}
			else
			{
				UNetConnection *RemoveNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
				check(IsValid(RemoveNetConnectionRef));
				UniquieNetIdRepl = RemoveNetConnectionRef->PlayerId;
			}
		}
		else
		{
			// Knows it is another player joining match
			UNetConnection *RemoveNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
			check(IsValid(RemoveNetConnectionRef));
			UniquieNetIdRepl = RemoveNetConnectionRef->PlayerId;
			
		}

		TSharedPtr<const FUniqueNetId> UniqueNetId = UniquieNetIdRepl.GetUniqueNetId();
		check(UniqueNetId != nullptr);
		IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(this->GetWorld());
		IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();

		// MainSession is what we created in the GameInstance when creating session 
		FName SessionNameToRegistar = FName("MainSession");
		UGameInstanceEOSTutorial *GameInstanceRef = Cast<UGameInstanceEOSTutorial>(GetWorld()->GetGameInstance());
		if (GameInstanceRef)
		{
			// Gets the session name from the game instance
			SessionNameToRegistar = GameInstanceRef->EOSSessionName;
		}

		// Bool to cehck if sessions was regstred with the session
		bool bRegistrationSuccess = SessionRef->RegisterPlayer(SessionNameToRegistar, *UniqueNetId, true);
		
		if(bRegistrationSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Registered Succesful"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Not Registered"));
		}
		
	}
	
}


