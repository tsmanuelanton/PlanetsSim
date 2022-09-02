// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseWrapper.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Chunk.generated.h"

UCLASS()
class PLANETSSIM_API AChunk : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChunk();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chunk Size")
	int ChunkSize = 1206;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD1PolySize = 1206;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD1VerticesQty = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD2PolySize = 603;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD2VerticesQty = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD2Distance = 45000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD3PolySize = 402;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD3VerticesQty = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOD Settings")
	int LOD3Distance = 15000;

	int PolySize = LOD1PolySize;
	int VerticesQty = LOD1VerticesQty;

	float UVScale = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	int HeighScale = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	int NoiseSeed = 1337;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	float NoiseFrequency = 0.0001;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	int32 NoiseOctaves = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	EFastNoise_NoiseType NoiseType = EFastNoise_NoiseType::SimplexFractal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	EFastNoise_Interp NoiseInterp = EFastNoise_Interp::Quintic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise")
	EFastNoise_FractalType NoiseFractalType = EFastNoise_FractalType::FBM;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasCollision = true;
	int Vertices = 0;
	int LOD = 2;
	bool spawned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> LandscapeMat;
	TArray<int32> Triangles;
	TArray<FVector> Vertex;
	TArray<FVector2d> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;

	virtual void SpawnChunk();
	virtual int DistanceToPlayer();
	void UpdateLOD(int Distance2Player);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;
	UFastNoiseWrapper* NoiseRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
