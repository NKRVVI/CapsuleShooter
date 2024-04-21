// Fill out your copyright notice in the Description page of Project Settings.


#include "MapItems/SpawnManager.h"
#include "Pawns/Enemy.h"
#include "MapItems/MapGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "MapItems/Tile.h"
#include "GameMode/CapsuleShooterGameMode.h"
#include "Engine/DataTable.h"
#include "Structs/LevelStruct.h"


// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/*
	starts spawning enemies on the map
*/
void ASpawnManager::StartSpawn(int32 Level)
{
	bStopSpawn = false;
	FLevelData* LevelData = LeveInfoTable->FindRow<FLevelData>(FName(FString::FromInt(Level)), TEXT(""));
	TimeBetweenSpawn = LevelData->SecondsBetweenSpawn;
	bInfiniteSpawn = LevelData->bInfiniteSpawn;

	StartSpawnTimer();
}

/*
	when this function is called, the next enemy to be spawned is spawned on the tile in the parameter
*/
void ASpawnManager::SpawnEnemyOnPlayer(ATile* Tile)
{
	bSpawnOnPlayer = true;
	PlayerTile = Tile;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	
	Cast<ACapsuleShooterGameMode>(UGameplayStatics::GetGameMode(this))->SetSpawnManager(this);
}


//Setting the unoccupied tiles so that enemies can be spawned on them
void ASpawnManager::ReceiveUnoccupiedTiles(TArray<ATile*>& _UnoccupiedTiles)
{
	UnoccupiedTiles = _UnoccupiedTiles;
	UE_LOG(LogTemp, Warning, TEXT("Unoccupied tiles received"));

}

// tile in the parameter starts flashing
void ASpawnManager::StartTileFlash(ATile* SpawnTile)
{
	SpawnTile->StartSpawn();
}

/*
	spawns enemy on given tile and stops that tile's flash
*/
void ASpawnManager::Spawn(ATile* SpawnTile)
{
	if (SpawnTile)
	{
		SpawnTile->StopSpawn();

		UE_LOG(LogTemp, Warning, TEXT("Spawn started"));

		if (World)
		{
			FTransform SpawnTransform = SpawnTile->GetActorTransform();

			if (AEnemy* Enemy = Cast<AEnemy>(World->SpawnActor<AEnemy>(EnemyClass)))
			{
				Enemy->SetActorLocation(SpawnTransform.GetLocation() /* + FVector(0, 0, 150)*/);
				Enemy->EnemyDeadDelegate.AddLambda([this]() {
					EnemyDiedDelegate.Broadcast();
					}
				);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("not spawned"));
			}
		}
	}
}

/*
	if the player is static for a certain amount of time, then the tile closest to the player is set to spawn, and that tile starts flashing
	otherwise a random tile is chosen from the unoccupied list to be set to spawn
*/
void ASpawnManager::StartSpawnTimer()
{
	if (bStopSpawn) return;

	ATile* NewSpawnTile;
	if (bSpawnOnPlayer)
	{
		NewSpawnTile = PlayerTile;
		bSpawnOnPlayer = false;
	}
	else
	{
		int32 RandomTileIndex = FMath::RandRange(0, UnoccupiedTiles.Num() - 1);
		NewSpawnTile = UnoccupiedTiles[RandomTileIndex];
	}

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::Spawn, NewSpawnTile);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnTimerDelegate, TimeOfFlashBeforeSpawn, false);

	StartTileFlash(NewSpawnTile);
	SpawnedEnemyDelegate.Broadcast();
	GetWorldTimerManager().SetTimer(SetSpawnTimerTimerHandle, this, &ThisClass::StartSpawnTimer, TimeBetweenSpawn);
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

