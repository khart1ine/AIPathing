// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShipAITestGameMode.h"
#include "ShipAITestPawn.h"

AShipAITestGameMode::AShipAITestGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AShipAITestPawn::StaticClass();
}
