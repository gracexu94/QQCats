// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "QQCats.h"
#include "QQCatsGameMode.h"
#include "QQCatsHUD.h"
#include "QQCatsCharacter.h"

AQQCatsGameMode::AQQCatsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AQQCatsHUD::StaticClass();
}
