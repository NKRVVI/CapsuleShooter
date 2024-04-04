// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CapsuleShooterGameInstance.h"
#include "GameMode/CapsuleShooterGameMode.h"
#include "Structs/LevelStruct.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/CapsulePlayer.h"
#include "HUD/CapsuleShooterHUD.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"


void UCapsuleShooterGameInstance::StartNewLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Max level is %s"), *FString::FromInt(MaxLevel));
	UE_LOG(LogTemp, Warning, TEXT("Set player health is %f"), Health);
	
	GameMode = Cast<ACapsuleShooterGameMode>(GetWorld()->GetAuthGameMode());

	if (Level > MaxLevel)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->GetHUD<ACapsuleShooterHUD>()->AddEndMenuWidgetToViewport();
		}
		
		return;
	}

	if (GameMode)
	{
		GameMode->WaveEnded.AddLambda(
			[this]() {
				Level++;
				Health = Cast<ACapsulePlayer>(UGameplayStatics::GetPlayerPawn(this, 0))->GetHealth();
				FTimerHandle ReloadTimerHandle;

				GetTimerManager().SetTimer(ReloadTimerHandle, this, &ThisClass::ReloadLevel, 2.f);
			}
		);

		GameMode->EnemyKilled.AddLambda(
			[this]() {
				NumEnemiesKilled++;
			}
		);
		int32 RandomIndex = FMath::RandRange(0, ColourPalettes.Num() - 1);
		GameMode->StartLevel(Level, *ColourPalettes[RandomIndex]);
		ColourPalettes.RemoveAt(RandomIndex);
	}
}

void UCapsuleShooterGameInstance::Init()
{
	Super::Init();

	MaxLevel = LevelInfoTable->GetRowNames().Num();
	UE_LOG(LogTemp, Warning, TEXT("Init"));
	ColourPaletteTable->GetAllRows(TEXT(""), ColourPalettes);
}

void UCapsuleShooterGameInstance::ResetGame()
{
	Level = 1;
	Health = 100.f;
	bAddOpeningMenu = false;
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()), false);
	ColourPalettes.Empty();
	ColourPaletteTable->GetAllRows(TEXT(""), ColourPalettes);
}

void UCapsuleShooterGameInstance::ReloadLevel()
{
	UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()), false);
	bAddOpeningMenu = false;
	UE_LOG(LogTemp, Warning, TEXT("Instance is starting level. Total number of enemies killed %d"), NumEnemiesKilled);
	UE_LOG(LogTemp, Warning, TEXT("Destroying map............................................."));
}
