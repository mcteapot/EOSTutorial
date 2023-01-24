		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameInstanceEOSTutorial.generated.h"

/**
 * 
 */
UCLASS()
class EOSTUTORIAL_API UGameInstanceEOSTutorial : public UGameInstance
{
	GENERATED_BODY()

public:

	/* VARABLES */

	/* Session search results ID */
	TShaderRef<FOnlineSessionSearch> SessionSearch;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "EOS|Properties")
	FString OpenLevelLocationText;


	
	/* LOGIN BLUEPRINT CALLABLE API */
	
	/** BP callable function for logginign into EOS */
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	/** Gets player user name if logged in to EOS */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	FString GetPlayerUsername();

	/** Gets player is logged in */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	bool IsPlayerLoggedIn();	

	
	//UFUNCTION(BlueprintCallable, Category = "EOS")
	//void LogoutWithEOS(); // TODO: Implement logout

	
	/* LOGIN BLUEPRINT CALLABLE API*/

	/** Creates a EOS session on deidcated server or land server */
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void CreateEOSSession(bool bIsDedicatedServer, bool bIsLanServer, int32 NumberOfPuglicConnections);

	/** Find a session to join */
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void FindSessionAndJoin();

	/** Joins sessions */
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void JoinSession();
	//void JoinSession(int32 SessionIndex);
	

	
	/* RETURN REFRENCE API */
	
	/** Return calls for after trying to connect for LoginWithEOS */
	void LoginWithEOSReturn(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	/** Return for on create session compleated */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/** Return finding a session */
	void OnFindSessionsComplete(bool bWasSuccessful);

	/** Return for joining a session */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
