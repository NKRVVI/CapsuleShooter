// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/LevelHeaderWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


void ULevelHeaderWidget::SetWaveNumberText(int32 Level)
{
	if (WaveNumberText)
	{
		WaveNumberText->SetText(FText::FromString(FString("Wave ") + FString::FromInt(Level)));
	}
}

void ULevelHeaderWidget::SetEnemiesSpawnedText(int32 NumEnemiesSpawned)
{
	if (EnemiesSpawnedText)
	{
		if (NumEnemiesSpawned == 0) EnemiesSpawnedText->SetText(FText::FromString(FString("Enemies spawned: Infinite")));
		else EnemiesSpawnedText->SetText(FText::FromString(FString("Enemies spawned: ") + FString::FromInt(NumEnemiesSpawned)));
	}
}
