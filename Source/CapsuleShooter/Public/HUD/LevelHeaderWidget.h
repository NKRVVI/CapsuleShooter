// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelHeaderWidget.generated.h"

class UTextBlock;
class UBorder;
class UWidgetAnimation;

UCLASS()
class CAPSULESHOOTER_API ULevelHeaderWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetWaveNumberText(int32 Level);
	void SetEnemiesSpawnedText(int32 NumEnemiesSpawned = 0);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ShowHeader"))
	void ShowHeader();

protected:

	UPROPERTY(EditDefaultsOnly)
	UWidgetAnimation* LifetimeAnimation;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveNumberText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemiesSpawnedText;

	UPROPERTY(meta = (BindWidget))
	UBorder* LevelHeaderBlock;
	
};
