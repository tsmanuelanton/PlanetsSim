// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Asignamos como componente raíz un ProceduralMeshComponent
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->bUseAsyncCooking = true;
	Mesh->SetCastShadow(false);
	SetRootComponent(Mesh);

	NoiseRef = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseWrapper"), false);
}

// Called when the game starts or when spawned

void AChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AChunk::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SpawnChunk();
	spawned = true;
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int Distance2Player = DistanceToPlayer();
	if (Distance2Player != -1)
	{
		UpdateLOD(Distance2Player);
	}
}

void AChunk::SpawnChunk()
{
	if (!spawned)
	{
		// Creamos el objeto que nos permite calcular el ruido de Perlin
		NoiseRef->SetupFastNoise(NoiseType, NoiseSeed, NoiseFrequency, NoiseInterp,
		                         NoiseFractalType, NoiseOctaves);
	}


	// FVector ChunkLocation = GetActorLocation();

	Triangles.Empty();
	Vertex.Empty();
	UVs.Empty();
	
	UKismetProceduralMeshLibrary::CreateGridMeshWelded(VerticesQty, VerticesQty, Triangles, Vertex, UVs,
	                                                   PolySize);


	for (int i = 0; i < Vertex.Num(); i++)
	{
		FVector Eleme = Vertex[i];
		Vertex[i].Set(Eleme.X, Eleme.Y,
		              NoiseRef->GetNoise2D(Eleme.X + GetActorLocation().X,
		                                   Eleme.Y + GetActorLocation().Y) * HeighScale);
	}

	Mesh->CreateMeshSection_LinearColor(0, Vertex, Triangles, normals, UVs, VertexColors, tangents, bHasCollision);

	Mesh->SetMaterial(0, LandscapeMat.Get());
}

// Devuelve la distance del Chunk al jugador
int AChunk::DistanceToPlayer()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();


	if (IsValid(PlayerPawn))
	{
		return FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());
	}

	return -1;
}

void AChunk::UpdateLOD(int Distance2Player)
{
	int NewLOD = -1;

	if (Distance2Player <= LOD3Distance)
	{
		NewLOD = 3;
		PolySize = LOD3PolySize;
		VerticesQty = LOD3VerticesQty;
	}
	else if (Distance2Player <= LOD2Distance)
	{
		NewLOD = 2;
		PolySize = LOD2PolySize;
		VerticesQty = LOD2VerticesQty;
	}
	else
	{
		NewLOD = 1;
		PolySize = LOD1PolySize;
		VerticesQty = LOD1VerticesQty;
	}


	if (LOD != NewLOD)
	{
		LOD = NewLOD;
		SpawnChunk();
	}
}
