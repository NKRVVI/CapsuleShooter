// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/LevelStruct.h"
#include "MapGenerator.generated.h"

class ANavemeshBoundsVolume;
class ACapsulePlayer;
class ANavMeshBoundsVolume;
class ATile;

DECLARE_MULTICAST_DELEGATE_OneParam(FSpawnOnPlayerDelegate, ATile* PlayerTile);
DECLARE_MULTICAST_DELEGATE(FPlayerDiedDelegate)

UCLASS()
class CAPSULESHOOTER_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();
	
	void GenerateMap(int32 Level, float PlayerHealth, const FColourPalette& ColourPalette);

	void DestroyMap();

	TArray<ATile*> UnoccupiedTiles;

	FSpawnOnPlayerDelegate SpawnOnPlayerDelegate;

	FPlayerDiedDelegate PlayerDiedDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 CheckFloodFillPath(FVector2D StartLocation, const TMap<FVector2D, ATile*>& TileMapRef, TMap<FVector2D, bool>& PathCheckMap);

	UPROPERTY(EditAnywhere, Category = "Map Generator Properties")
	int32 NumOfTiles;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

	TMap<FVector2D, ATile*> TileCoordMap;

	UPROPERTY(EditInstanceOnly, Category = "Map Generator Properties")
	float ObstaclePercent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map Generator Properties")
	TArray<ATile*> TileMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACapsulePlayer> CapsulePlayerClass;

	ACapsulePlayer* CapsulePlayer;

	UPROPERTY(EditAnywhere)
	bool bIsActivated = true;

	UPROPERTY(EditAnywhere)
	class UDataTable* LevelInfoTable;

	UWorld* World;

	FVector PlayerCurrentPosition = FVector::ZeroVector;
	float TileLength;
	UPROPERTY(EditDefaultsOnly)
	float PlayerMovementCheckTime = 2.f;
	bool bSpawnOnPlayer = false;
	void CheckIfPlayerStatic();
	FTimerHandle CheckPlayerMovementTimerHandle;

	UPROPERTY(EditAnywhere)
	float YOffset = 0.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
