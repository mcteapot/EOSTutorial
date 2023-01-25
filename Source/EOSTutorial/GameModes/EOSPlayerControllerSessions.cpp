// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSPlayerControllerSessions.h"

void AEOSPlayerControllerSessions::OnNetCleanup(UNetConnection* Connection)
{

	// Super should be called last
	Super::OnNetCleanup(Connection);
}
