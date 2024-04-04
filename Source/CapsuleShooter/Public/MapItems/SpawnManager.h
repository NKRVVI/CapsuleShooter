// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FSpawnedEnemy);
DECLARE_MULTICAST_DELEGATE(FEnemyDied)

class AEnemy;
class ATile;
class AMapGenerator;

UCLASS()
class CAPSULESHOOTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	FORCEINLINE void SetUnoccupiedTiles(TArray<ATile*>& _UnoccupiedTiles) { UnoccupiedTiles = _UnoccupiedTiles; };

	FORCEINLINE void StopSpawn() { bStopSpawn = true; };

	void StartSpawn(int32 Level);

	FSpawnedEnemy SpawnedEnemyDelegate;

	FEnemyDied EnemyDiedDelegate;

	void SpawnEnemyOfPlayer(ATile* Tile);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveUnoccupiedTiles(TArray<ATile*>& _UnoccupiedTiles);

	void StartTileFlash(ATile* SpawnTile);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpawn; 

	UPROPERTY(EditDefaultsOnly)
	float TimeOfFlashBeforeSpawn;

	void Spawn(ATile* SpawnTile);
	void StartSpawnTimer();

	FTimerHandle SpawnTimer;
	FTimerHandle TileFlashTimer;
	FTimerHandle SetSpawnTimerTimerHandle;

	bool bStopSpawn = false;

	TArray<class ATile*> UnoccupiedTiles;

	UPROPERTY(EditInstanceOnly)
	AMapGenerator* MapGenerator;

	UPROPERTY(EditAnywhere)
	class UDataTable* LeveInfoTable;

	UWorld* World;

	bool bSpawnOnPlayer = false;
	ATile* PlayerTile;

	bool bInfiniteSpawn = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE  void SpeedUpSpawn() { TimeBetweenSpawn = FMath::Clamp(TimeBetweenSpawn * 0.75, 0.1, TimeBetweenSpawn); }

};
