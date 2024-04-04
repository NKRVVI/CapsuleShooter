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

void ASpawnManager::StartSpawn(int32 Level)
{
	bStopSpawn = false;
	FLevelData* LevelData = LeveInfoTable->FindRow<FLevelData>(FName(FString::FromInt(Level)), TEXT(""));
	TimeBetweenSpawn = LevelData->SecondsBetweenSpawn;
	bInfiniteSpawn = LevelData->bInfiniteSpawn;

	StartSpawnTimer();
}

void ASpawnManager::SpawnEnemyOfPlayer(ATile* Tile)
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

void ASpawnManager::ReceiveUnoccupiedTiles(TArray<ATile*>& _UnoccupiedTiles)
{
	UnoccupiedTiles = _UnoccupiedTiles;
	UE_LOG(LogTemp, Warning, TEXT("Unoccupied tiles received"));

}

void ASpawnManager::StartTileFlash(ATile* SpawnTile)
{
	SpawnTile->StartSpawn();
}

void ASpawnManager::Spawn(ATile* SpawnTile)
{
	if (SpawnTile)
	{
		SpawnTile->StopSpawn();

		UE_LOG(LogTemp, Warning, TEXT("Spawn started"));
		//if (bStopSpawn) return;

		if (World)
		{
			//
			//FTransform SpawnTransform = UnoccupiedTiles[RandomTileIndex]->GetActorTransform();

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

	/*if (bStopSpawn) return;

	int32 RandomTileIndex = FMath::RandRange(0, UnoccupiedTiles.Num() - 1);
	ATile* NewSpawnTile = UnoccupiedTiles[RandomTileIndex];

	FTimerDelegate SpawnTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::Spawn, NewSpawnTile);
	GetWorldTimerManager().SetTimer(SpawnTimer, SpawnTimerDelegate, TimeBetweenSpawn, false);

	FTimerDelegate TileFlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StartTileFlash, NewSpawnTile);
	//NewSpawnTile->StartSpawn();
	GetWorldTimerManager().SetTimer(TileFlashTimer, TileFlashTimerDelegate,FMath::Max(0.1, TimeBetweenSpawn - TimeOfFlashBeforeSpawn), false);*/
}

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

	//FTimerDelegate TileFlashTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::StartTileFlash, NewSpawnTile);
	//GetWorldTimerManager().SetTimer(TileFlashTimer, TileFlashTimerDelegate, TimeBetweenSpawn, false);
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

