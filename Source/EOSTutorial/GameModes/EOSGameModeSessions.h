// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOSGameModeSessions.generated.h"

/**
 * 
 */
UCLASS()
class EOSTUTORIAL_API AEOSGameModeSessions : public AGameModeBase
{
	GENERATED_BODY()

	/** Constructure function of class */
	AEOSGameModeSessions();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
