// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOSPlayerControllerSessions.generated.h"

/**
 * 
 */
UCLASS()
class EOSTUTORIAL_API AEOSPlayerControllerSessions : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnNetCleanup(UNetConnection* Connection) override;
	
};
