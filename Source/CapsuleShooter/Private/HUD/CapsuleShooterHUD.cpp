// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CapsuleShooterHUD.h"
#include "HUD/HealthBarWidget.h"
#include "HUD/LevelHeaderWidget.h"
#include "HUD/OpeningMenu.h"
#include "GameMode/CapsuleShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/EndMenu.h"

void ACapsuleShooterHUD::AddHealthBarWidgetToViewport()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && HealthBarWidgetClass)
		{
			HealthBarWidget = CreateWidget<UHealthBarWidget>(PlayerController, HealthBarWidgetClass);
			HealthBarWidget->AddToViewport();
		}
	}
}

void ACapsuleShooterHUD::AddLevelHeaderWidgetToViewport()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && LevelHeaderWidgetClass)
		{
			LevelHeaderWidget = CreateWidget<ULevelHeaderWidget>(PlayerController, LevelHeaderWidgetClass);
			LevelHeaderWidget->AddToViewport();
		}
	}
}

void ACapsuleShooterHUD::AddEndMenuWidgetToViewport()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && EndMenuClass)
		{
			EndMenuWidget = CreateWidget<UEndMenu>(PlayerController, EndMenuClass);
			EndMenuWidget->AddToViewport();
			if(HealthBarWidget) HealthBarWidget->RemoveFromViewport();
		}
	}
}

void ACapsuleShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (EndMenuWidget) EndMenuWidget->RemoveFromViewport();

	UCapsuleShooterGameInstance* GameInstance = Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance->AddOpeningMenu())
	{
		UE_LOG(LogTemp, Warning, TEXT("Add opening menu %d"), false);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Add"));
	}

	if (UWorld* World = GetWorld())
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && OpeningMenuClass)
		{
			OpeningMenuWidget = CreateWidget<UOpeningMenu>(PlayerController, OpeningMenuClass);
			OpeningMenuWidget->AddToViewport();
			if (GameInstance)
			{
				GameInstance->PlayMenuMusic();
			}
		}
	}
}
