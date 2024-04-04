#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Structs/LevelStruct.h"
#include "CapsuleShooterGameMode.generated.h"

class AMapGenerator;
class ASpawnManager;

DECLARE_MULTICAST_DELEGATE(FWaveEnded)
DECLARE_MULTICAST_DELEGATE(FEnemyKilled)
DECLARE_MULTICAST_DELEGATE(FStopEnemies)

UCLASS()
class CAPSULESHOOTER_API ACapsuleShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACapsuleShooterGameMode();
	virtual void BeginPlay() override;
	void StartLevel(int32 Level, const FColourPalette& ColourPalette);

	FWaveEnded WaveEnded;
	FEnemyKilled EnemyKilled;
	FStopEnemies StopEnemies;

	bool bShouldMove = true;
protected:
	UPROPERTY(EditAnywhere)
	int32 NumEnemiesToBeSpawnedPerWave;
	
	UPROPERTY(BlueprintReadOnly)
	int32 NumEnemiesSpawned = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 EnemiesKilledInWave = 0;

	AMapGenerator* MapGenerator;
	ASpawnManager* SpawnManager;

	UPROPERTY(EditAnywhere)
	class UDataTable* LevelInfoTable;

	bool bInfiniteSpawn;

	UPROPERTY(EditAnywhere)
	int32 ReductionRange = 30;

public:
	void SetMapGenerator(AMapGenerator* _MapGenerator);
	void SetSpawnManager(ASpawnManager* _SpawnManager);
};