// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CapsuleShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MapItems/MapGenerator.h"
#include "MapItems/SpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/CapsuleShooterGameInstance.h"
#include "HUD/CapsuleShooterHUD.h"
#include "HUD/LevelHeaderWidget.h"
#include "HUD/EndMenu.h"


ACapsuleShooterGameMode::ACapsuleShooterGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACapsuleShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));
}


/*
	the map is generated, and the spawnmanager starts spawning enemies
	the gamemode also keeps track of enemies spawned and enemies killed which are communicate to by the map manaager
	and the spawn manager
*/
void ACapsuleShooterGameMode::StartLevel(int32 Level, const FColourPalette& ColourPalette)
{
	UE_LOG(LogTemp, Warning, TEXT("Game mode is starting level"));

	FLevelData* LevelData = LevelInfoTable->FindRow<FLevelData>(FName(FString::FromInt(Level)), TEXT(""));
	NumEnemiesToBeSpawnedPerWave = LevelData->EnemiesToBeSpawned;
	bInfiniteSpawn = LevelData->bInfiniteSpawn;

	if (MapGenerator && SpawnManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gamemode and spawnmanager are both valid"));
		float PlayerHealth = Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->GetPlayerHealth();
		UE_LOG(LogTemp, Warning, TEXT("Inital player health is %f"), PlayerHealth);
		MapGenerator->GenerateMap(Level, PlayerHealth, ColourPalette);

		MapGenerator->SpawnOnPlayerDelegate.AddLambda(
			[this](ATile* Tile) {
				SpawnManager->SpawnEnemyOnPlayer(Tile);
			}
		);

		MapGenerator->PlayerDiedDelegate.AddLambda(
			[this]()
			{
				if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
				{
					ACapsuleShooterHUD* HUD = PlayerController->GetHUD<ACapsuleShooterHUD>();
					HUD->AddEndMenuWidgetToViewport();
					int32 CapsuleScore = Cast<UCapsuleShooterGameInstance>(GetGameInstance())->GetScore();
					HUD->GetEndMenuWidget()->SetScore(FText::FromString(FString::FromInt(CapsuleScore)));
					StopEnemies.Broadcast();
					SpawnManager->StopSpawn();
					bShouldMove = false;
				}
			}
		);

		SpawnManager->SetUnoccupiedTiles(MapGenerator->UnoccupiedTiles);
		SpawnManager->SpawnedEnemyDelegate.AddLambda(
			[this]() {
				NumEnemiesSpawned++;
				if (bInfiniteSpawn)
				{
					if (NumEnemiesSpawned > 0 && NumEnemiesSpawned % ReductionRange == 0)
					{
						SpawnManager->SpeedUpSpawn();
					}
					return;
				}
				if (NumEnemiesSpawned == NumEnemiesToBeSpawnedPerWave)
				{
					SpawnManager->StopSpawn();
				}
			}
		);

		SpawnManager->EnemyDiedDelegate.AddLambda(
			[this]() {
				EnemiesKilledInWave++;
				EnemyKilled.Broadcast();
				if (bInfiniteSpawn) return;
				if (EnemiesKilledInWave == NumEnemiesToBeSpawnedPerWave)
				{
					WaveEnded.Broadcast();
				}
			}
		);
		SpawnManager->StartSpawn(Level);

		// Player level header widget
		ACapsuleShooterHUD* HUD = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<ACapsuleShooterHUD>();
		HUD->AddLevelHeaderWidgetToViewport();
		ULevelHeaderWidget* LevelHeaderWidget = HUD->GetLevelHeaderWidget();
		LevelHeaderWidget->SetWaveNumberText(Level);
		if(bInfiniteSpawn) LevelHeaderWidget->SetEnemiesSpawnedText();
		else LevelHeaderWidget->SetEnemiesSpawnedText(NumEnemiesToBeSpawnedPerWave);
		LevelHeaderWidget->ShowHeader();
	}
}

void ACapsuleShooterGameMode::SetMapGenerator(AMapGenerator* _MapGenerator)
{
	MapGenerator = _MapGenerator;
	bool bStartLevel = !Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->AddOpeningMenu();

	if (MapGenerator && SpawnManager && bStartLevel)
	{
		Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->StartNewLevel();
	}
}

void ACapsuleShooterGameMode::SetSpawnManager(ASpawnManager* _SpawnManager)
{
	SpawnManager = _SpawnManager;
	bool bStartLevel = !Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->AddOpeningMenu();
	if (MapGenerator && SpawnManager && bStartLevel)
	{
		Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->StartNewLevel();
	}
}
