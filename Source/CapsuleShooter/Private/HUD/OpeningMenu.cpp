// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OpeningMenu.h"
#include "Components/Button.h"
#include "GameMode/CapsuleShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UOpeningMenu::NativeConstruct()
{
	Super::NativeConstruct();

	BeginButton->OnClicked.AddDynamic(this, &ThisClass::BeginButtonPressed);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonPressed);
}

void UOpeningMenu::BeginButtonPressed()
{
	UCapsuleShooterGameInstance* GameInstance = Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->StartNewLevel();
	RemoveFromViewport();
	GameInstance->PlayMainMusic();
}

void UOpeningMenu::ExitButtonPressed()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
}
