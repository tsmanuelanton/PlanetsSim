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
			FIntVector ChunkCoords = FIntVector(GetChunkCoords().X + TempX, GetChunkCoords().Y + TempY, 0);
			float currentDistance = FVector(ChunkCoords.X * ChunkSize, ChunkCoords.Y * ChunkSize, 0).Length();

			if (currentDistance <= SpawnDistance * ChunkSize && !CurrentSpawnedChunkCoords.Contains(ChunkCoords))
			{
				FVector Location(ChunkCoords.X * ChunkSize, ChunkCoords.Y * ChunkSize, 0);
				FRotator Rotator(0, 0, 0);
				FActorSpawnParameters SpawnInfo;
				AActor* SpawnActorRef = GetWorld()->SpawnActor<AChunk>(Location, Rotator, SpawnInfo);

				CurrentSpawnedChunkActors.Add(SpawnActorRef);
				CurrentSpawnedChunkCoords.Add(ChunkCoords);
			}
		}
	}
}

void AChunkGenerator::DestroyChunks()
{
	for (int i = 0; i < CurrentSpawnedChunkCoords.Num(); i++)
	{
		FIntVector Coord = CurrentSpawnedChunkCoords[i];

		auto distance = FVector::Distance(FVector(Coord.X * ChunkSize, Coord.Y * ChunkSize, 0),
		                                  GetPawn()->GetActorLocation());
		if (distance > SpawnDistance * ChunkSize)
		{
			CurrentSpawnedChunkActors[i]->Destroy();
			CurrentSpawnedChunkActors.RemoveAt(i);
			CurrentSpawnedChunkCoords.RemoveAt(i);
		}
	}
}
