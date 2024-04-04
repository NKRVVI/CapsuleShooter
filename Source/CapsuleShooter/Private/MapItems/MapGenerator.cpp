// Fill out your copyright notice in the Description page of Project Settings.


#include "MapItems/MapGenerator.h"
#include "MapItems/Tile.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Pawns/CapsulePlayer.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "GameMode/CapsuleShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Structs/LevelStruct.h"
#include "Items/Gun.h"
#include "Camera/CameraActor.h"


// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMapGenerator::GenerateMap(int32 Level, float PlayerHealth, const FColourPalette& ColourPalette)
{
	UE_LOG(LogTemp, Warning, TEXT("Row name is %s"), *FString::FromInt(Level));

	FLevelData* LevelData = LevelInfoTable->FindRow<FLevelData>(FName(FString::FromInt(Level)), TEXT(""));
	NumOfTiles = LevelData->NumTiles;
	ObstaclePercent = LevelData->ObstaclePercent;

	World = GetWorld();
	if (World && TileClass)
	{
		TileMap.SetNum(NumOfTiles * NumOfTiles);

		for (int i = 0; i < NumOfTiles; i++)
		{
			for (int j = 0; j < NumOfTiles; j++)
			{
				TileMap[NumOfTiles * i + j] = World->SpawnActor<ATile>(TileClass);
				FVector TileLocation = GetActorLocation();
				TileLocation.X += TileMap[NumOfTiles * i + j]->GetTileBounds().X * 2 * (i + 0.5);
				TileLocation.Y += TileMap[NumOfTiles * i + j]->GetTileBounds().Y * 2 * (j + 0.5);

				TileMap[NumOfTiles * i + j]->SetActorLocation(TileLocation);
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *TileLocation.ToString());

				TileCoordMap.Add(FVector2D(i, j), TileMap[NumOfTiles * i + j]);
			}
		}
	}
	else if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World does not exist"));
	}
	else if (!TileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tile class does not exist"));
	}

	TileLength = TileMap[0]->GetTileBounds().X * 2;

	FVector2D Center = FVector2D(TileLength * NumOfTiles / 2);

	AActor* Camera = UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass());
	Camera->SetActorLocation(FVector(Center.X - YOffset, Center.Y, Camera->GetActorLocation().Z));

	//UE_LOG(LogTemp, Warning, TEXT("Tilemap length : %d"), TileMap.Num());

	/*
	Fisher-Yates shuffle algorithm in order to shuffle the tiles and choose the ones that will become obstacles
	-- To shuffle an array a of n elements (indices 0..n-1):
	for i from n−1 down to 1 do
		j ← random integer such that 0 ≤ j ≤ i
		exchange a[j] and a[i]
	*/

	for (int i = TileMap.Num() - 1; i >= 1; i--)
	{
		int j = FMath::RandRange(0, i);
		ATile* SwapTile = TileMap[i];
		TileMap[i] = TileMap[j];
		TileMap[j] = SwapTile;
	}

	int32 NumObstacles = NumOfTiles * NumOfTiles * ObstaclePercent;

	int32 NumFreeTiles = 0;
	int32 ObstaclesPlaced = 0;

	TMap<FVector2D, bool> PathCheckMap;
	for (int i = 0; i < NumOfTiles; i++)
	{
		for (int j = 0; j < NumOfTiles; j++)
		{
			PathCheckMap.Add(FVector2D(i, j), false);
		}
	}

	for (int i = 0; i < TileMap.Num(); i++)
	{
		if (*TileCoordMap.FindKey(TileMap[i]) == FVector2D(int32(NumOfTiles / 2))) continue;


		if (ObstaclesPlaced < NumObstacles)
		{
			TileMap[i]->BecomeObstacle();
			ObstaclesPlaced++;

			auto TempPathCheckMap = PathCheckMap;

			float CheckFloodFill = CheckFloodFillPath(FVector2D(int32(NumOfTiles / 2)), TileCoordMap, TempPathCheckMap);

			//UE_LOG(LogTemp, Warning, TEXT("%d, %d"), CheckFloodFill, NumObstacles);
			if (CheckFloodFill == NumOfTiles * NumOfTiles - ObstaclesPlaced)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s becomes obstacle"), *(*TileCoordMap.FindKey(Tile)).ToString());
				//if (ObstaclesPlaced == NumObstacles) break;
				FVector2D Coord = *TileCoordMap.FindKey(TileMap[i]);
				FLinearColor InterpColour = FMath::Lerp<FLinearColor>(ColourPalette.StartColour, ColourPalette.EndColour, float(Coord.X) / float(NumOfTiles));
				TileMap[i]->SetObstacleColour(InterpColour);
				//UE_LOG(LogTemp, Warning, TEXT("InterpColour is %f"), Coord.X / NumOfTiles);
			}
			else
			{
				TileMap[i]->UnbecomeObstacle();
				UnoccupiedTiles.Add(TileMap[i]);
				ObstaclesPlaced--;
				//UE_LOG(LogTemp, Warning, TEXT("%s is skipped"), *(*TileCoordMap.FindKey(Tile)).ToString());
			}
		}
		else
		{
			UnoccupiedTiles.Add(TileMap[i]);
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("%d, %d"), NumObstacles, ObstaclesPlaced);

	if (World)
	{
		FTransform SpawnTransform = TileCoordMap[FVector2D((int32)(NumOfTiles / 2))]->GetActorTransform();
		CapsulePlayer = World->SpawnActor<ACapsulePlayer>(CapsulePlayerClass);

		FVector SpawnLocation = SpawnTransform.GetLocation();
		FRotator SpawnRotation = FRotator(SpawnTransform.GetRotation());
		UE_LOG(LogTemp, Warning, TEXT("Tile location is %s"), *SpawnLocation.ToString());

		CapsulePlayer->SetActorLocation(SpawnLocation);
		CapsulePlayer->SetHealth(PlayerHealth);
		CapsulePlayer->SetGunClass(LevelInfoTable->FindRow<FLevelData>(FName(FString::FromInt(Level)), TEXT(""))->GunClass);

		CapsulePlayer->PlayerDiedDelegate.AddLambda(
			[this]()
			{
				PlayerDiedDelegate.Broadcast();
			}
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("Number of unoccupied tiles is %d"), UnoccupiedTiles.Num());
	UE_LOG(LogTemp, Warning, TEXT("Number of occupied tiles is %d"), ObstaclesPlaced);

	CheckIfPlayerStatic();
}

void AMapGenerator::DestroyMap()
{
	for (auto& Tile : TileMap)
	{
		Tile->Destroy();
	}

	TileMap.Empty();

	TileCoordMap.Empty();
	UnoccupiedTiles.Empty();

	CapsulePlayer->DestroySelf();
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	Cast<ACapsuleShooterGameMode>(UGameplayStatics::GetGameMode(this))->SetMapGenerator(this);
}


int32 AMapGenerator::CheckFloodFillPath(FVector2D StartLocation, const TMap<FVector2D, ATile*>& TileMapRef, TMap<FVector2D, bool>& PathCheckMap)
{
	int32 Path = 1;
	PathCheckMap[StartLocation] = true;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if ((i == 0 && j != 0) || (i != 0 && j == 0))
			{
				FVector2D SearchLocation = StartLocation + FVector2D(i, j);

				if (SearchLocation.X < 0 || SearchLocation.X >= NumOfTiles) continue;
				if (SearchLocation.Y < 0 || SearchLocation.Y >= NumOfTiles) continue;

				if (!TileMapRef[SearchLocation]->IsObstacle() && !PathCheckMap[SearchLocation])
				{
					Path += CheckFloodFillPath(SearchLocation, TileMapRef, PathCheckMap);
				}
			}
		}
	}

	return Path;
}

void AMapGenerator::CheckIfPlayerStatic()
{
	if ((PlayerCurrentPosition - CapsulePlayer->GetActorLocation()).Size() <= TileLength)
	{
		bSpawnOnPlayer = true;
		FVector MapGeneratorLocation = GetActorLocation();
		FVector PlayerLocation = CapsulePlayer->GetActorLocation();
		FVector2D TileCoord;
		TileCoord.X = FMath::FloorToInt32(FMath::Abs(MapGeneratorLocation.X - PlayerLocation.X) / TileLength);
		TileCoord.Y = FMath::FloorToInt32(FMath::Abs(MapGeneratorLocation.Y - PlayerLocation.Y) / TileLength);

		SpawnOnPlayerDelegate.Broadcast(*TileCoordMap.Find(TileCoord));
		UE_LOG(LogTemp, Warning, TEXT("Broadcasting"));
	}

	PlayerCurrentPosition = CapsulePlayer->GetActorLocation();
	GetWorldTimerManager().SetTimer(CheckPlayerMovementTimerHandle, this, &ThisClass::CheckIfPlayerStatic, PlayerMovementCheckTime);
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}