// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "VehicleMovementGameMode.h"
#include "VehicleMovementPawn.h"
#include "VehicleMovementHud.h"

AVehicleMovementGameMode::AVehicleMovementGameMode()
{
	DefaultPawnClass = AVehicleMovementPawn::StaticClass();
	HUDClass = AVehicleMovementHud::StaticClass();
}
