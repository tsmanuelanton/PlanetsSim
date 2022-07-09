// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetsSimGameModeBase.h"

APlanetsSimGameModeBase::APlanetsSimGameModeBase() : Super()
{
	// Asignamos como DefaultPawn nuestro pesonaje
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClassAsset(
		TEXT("/Game/Blueprints/OLD_BP_ChunkGenerator"));
	if (ControllerClassAsset.Succeeded())
	{
		PlayerControllerClass = ControllerClassAsset.Class;
	}
}


