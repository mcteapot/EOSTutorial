// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSPlayerControllerSessions.h"

#include "GameInstanceEOSTutorial.h"

void AEOSPlayerControllerSessions::OnNetCleanup(UNetConnection* Connection)
{

	// Call to game instance
	UGameInstanceEOSTutorial *GameInstanceRef = Cast<UGameInstanceEOSTutorial>(GetWorld()->GetGameInstance());

	if(GameInstanceRef)
	{
		GameInstanceRef->DestroySession(); // Need to have a way to see if the player is host or not
	}
	
	// Super should be called last
	Super::OnNetCleanup(Connection);
}
