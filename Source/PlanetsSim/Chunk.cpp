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

	// Creamos el objeto que nos permite calcular el ruido de Perlin
	NoiseRef = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseWrapper"), false);
	NoiseRef->SetupFastNoise(EFastNoise_NoiseType::SimplexFractal, 1337, 0.0001, EFastNoise_Interp::Quintic,
	                         EFastNoise_FractalType::FBM, 6);

	// static ConstructorHelpers::FObjectFinder<UMaterial> LandscapeMaterial(TEXT("/Game/BP_MatLandascape_Inst"));
	//
	// if (LandscapeMaterial.Succeeded())
	// {
	// 	LandscapeMat = LandscapeMaterial.Object;
	// 	
	// }
	// else
	// 	if(GEngine)
	// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Material no encontrado"));

	// LandscapeMat = new UMaterialInterface();
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
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChunk::SpawnChunk()
{
	FVector ChunkLocation = GetActorLocation();

	Triangles.Empty();
	Vertex.Empty();
	UVs.Empty();

	// for (int y = 0, i = 0; y < VerticesQuantity; y++)
	// {
	// 	for (int x = 0; x < VerticesQuantity - 1; x++, i++)
	// 	{
	// 		float NoiseAtLocation = NoiseRef->
	// 			GetNoise2D(ChunkLocation.X + x * PolySize, ChunkLocation.Y + y * PolySize);
	//
	// 		// Create vertex
	// 		Vertex.Add(FVector(
	// 			(x + (VerticesQuantity - 1) / -2.0f) * PolySize,
	// 			(y + (VerticesQuantity - 1) / 2.0f) * PolySize,
	// 			NoiseAtLocation * HeighScale));
	//
	// 		if (x < VerticesQuantity - 1 && y < VerticesQuantity - 1)
	// 		{
	// 			// Create triangles
	// 			Triangles.Add(i);
	// 			Triangles.Add(i + VerticesQuantity);
	// 			Triangles.Add(i + 1);
	// 			Triangles.Add(i + 1);
	// 			Triangles.Add(i + VerticesQuantity);
	// 			Triangles.Add(i + VerticesQuantity + 1);
	// 		}
	//
	// 		// Create UVMap
	// 		// UVs.Add(FVector2d(TempX / (VerticesQuantity - 1), TempY / (VerticesQuantity - 1)));
	//
	// 		Vertices++;
	// 	}
	// }

	// KismetProceduralMeshLibrary::create_grid_mesh_split()

	UKismetProceduralMeshLibrary::CreateGridMeshWelded(VerticesQuantity, VerticesQuantity, Triangles, Vertex, UVs,
	                                                   PolySize);

	for (int i = 0; i < Vertex.Num(); i++)
	{
		FVector Eleme = Vertex[i];
		Vertex[i].Set(Eleme.X, Eleme.Y,
		              NoiseRef->GetNoise2D(Eleme.X + GetActorLocation().X,
		                                   Eleme.Y + GetActorLocation().Y) * HeighScale);
	}

	Mesh->CreateMeshSection_LinearColor(0, Vertex, Triangles, normals, UVs, VertexColors, tangents, true);

	if (LandscapeMat)
		Mesh->SetMaterial(0, LandscapeMat);
	
}
