// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/LevelStruct.h"
#include "CapsuleShooterGameInstance.generated.h"

/**
 * 
 */

class ACapsuleShooterGameMode;

UCLASS()
class CAPSULESHOOTER_API UCapsuleShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void StartNewLevel();

	virtual void Init() override;

	void ResetGame();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMenuMusic();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMainMusic();
protected:

	void ReloadLevel();

	ACapsuleShooterGameMode* GameMode;

	int32 NumEnemiesKilled = 0;

	int32 Level = 1;

	int32 MaxLevel;

	UPROPERTY(EditDefaultsOnly)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* LevelInfoTable;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ColourPaletteTable;

	bool bAddOpeningMenu = true;

	TArray<FColourPalette*> ColourPalettes;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* MenuMusic;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* GameMusic;

	UPROPERTY(BlueprintReadWrite)
	class UAudioComponent* MenuAudioComponent;

	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* MainAudioComponent;

public:
	float GetPlayerHealth() { return Health; }
	bool AddOpeningMenu() { return bAddOpeningMenu; }
	FORCEINLINE int32 GetScore() const { return NumEnemiesKilled; }
};
