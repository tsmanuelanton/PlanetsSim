// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkGenerator.h"

#include "Chunk.h"

AChunkGenerator::AChunkGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AChunkGenerator::BeginPlay()
{
	Super::BeginPlay();
	SpawnNewChunk();
}

// Si el jugador cambia de chunk, cabiamos las coords del chunk que estamos
void AChunkGenerator::Tick(float DeltaSeconds)
{
	FIntVector ChunkCoord = GetChunkCoords();
	if (ChunkCoord.X != LastChunkCoords.X || ChunkCoord.Y != LastChunkCoords.Y)
	{
		LastChunkCoords = ChunkCoord;
		CoordsHaveChanged();
	}
}

void AChunkGenerator::CoordsHaveChanged()
{
	DestroyChunks();
	SpawnNewChunk();
}


FIntVector AChunkGenerator::GetChunkCoords()
{
	FIntVector ChunkCoords = FIntVector(
		FMath::RoundToInt(GetPawn()->GetActorLocation().X / ChunkSize),
		FMath::RoundToInt(GetPawn()->GetActorLocation().Y / ChunkSize),
		0
	);
	return ChunkCoords;
}

void AChunkGenerator::SpawnNewChunk()
{
	for (int TempX = -SpawnDistance; TempX < SpawnDistance; TempX++)
	{
		for (int TempY = -SpawnDistance; TempY < SpawnDistance; TempY++)
		{
			// Calculamos las cordenadas de un posible nuevo chunk
			FIntVector NewChunkCoords = FIntVector(GetChunkCoords().X + TempX, GetChunkCoords().Y + TempY, 0);
			float currentDistance = FVector(TempX * ChunkSize, TempY * ChunkSize, 0).Length();

			// Si la distancia a este nuevo chunk es mayor que la distancia de spawn, lo descartamos
			if (currentDistance <= SpawnDistance * ChunkSize && !CurrentSpawnedChunkCoords.Contains(NewChunkCoords))
			{
				// Si el chunk está dentro de la distancia de spawn, creamos un nuevo actor Chunk en las coords calculadas
				FVector Location(NewChunkCoords.X * ChunkSize, NewChunkCoords.Y * ChunkSize, 0);
				FRotator Rotator(0, 0, 0);
				FActorSpawnParameters SpawnInfo;
				AActor* SpawnActorRef = GetWorld()->SpawnActor<AChunk>(Location, Rotator, SpawnInfo);

				CurrentSpawnedChunkActors.Add(SpawnActorRef);
				CurrentSpawnedChunkCoords.Add(NewChunkCoords);
			}
		}
	}
}

void AChunkGenerator::DestroyChunks()
{
	for (int i = 0; i < CurrentSpawnedChunkCoords.Num(); i++)
	{
		FIntVector ChunkCoords = CurrentSpawnedChunkCoords[i];

		// Si la distancia entre el jugador y el chunk es mayor que la distancia de spawn, lo eliminamos
		auto distance = FVector::Distance(FVector(ChunkCoords.X * ChunkSize, ChunkCoords.Y * ChunkSize, 0),
		                                  GetPawn()->GetActorLocation());
		if (distance > SpawnDistance * ChunkSize)
		{
			CurrentSpawnedChunkActors[i]->Destroy();
			CurrentSpawnedChunkActors.RemoveAt(i);
			CurrentSpawnedChunkCoords.RemoveAt(i);
		}
	}
}
