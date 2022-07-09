// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetsSimGameModeBase.h"

APlanetsSimGameModeBase::APlanetsSimGameModeBase() : Super()
{
	// Asignamos como DefaultPawn nuestro pesonaje
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacterBP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClassAsset(
		TEXT("/Game/Blueprints/ChunkRuntimeGeneratorBP"));
	if (ControllerClassAsset.Succeeded())
	{
		PlayerControllerClass = ControllerClassAsset.Class;
	}
}


