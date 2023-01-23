		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceEOSTutorial.generated.h"

/**
 * 
 */
UCLASS()
class EOSTUTORIAL_API UGameInstanceEOSTutorial : public UGameInstance
{
	GENERATED_BODY()

public:

	/** BP callable function for logginign into EOS */
	UFUNCTION(BlueprintCallable, Category = "EOS")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);

	/** Gets player user name if logged in to EOS */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	FString GetPlayerUsername();

	/** Gets player is logged in */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOS")
	bool IsPlayerLoggedIn();	
	
	/** Return calls for after trying to connect for LoginWithEOS */
	void LoginWithEOSReturn(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error); 
};
