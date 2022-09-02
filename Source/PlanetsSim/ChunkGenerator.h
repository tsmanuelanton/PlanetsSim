// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "GameFramework/PlayerController.h"
#include "ChunkGenerator.generated.h"

/**
 * 
 */
UCLASS()
class PLANETSSIM_API AChunkGenerator : public APlayerController
{
	GENERATED_BODY()

public:

	AChunkGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chunk Settings")
	int SpawnDistance = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chunk Settings")
	int ChunkSize = 1206;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chunk Settings")
	TSubclassOf<AActor> ChunkClass;

	TArray<FIntVector> CurrentSpawnedChunkCoords;
	TArray<AActor*> CurrentSpawnedChunkActors;
	FIntVector LastChunkCoords;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)

	virtual FIntVector GetChunkCoords();
	virtual void CoordsHaveChanged();
	virtual void SpawnNewChunk();
	virtual void DestroyChunks();
};
