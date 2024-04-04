// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EndMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameMode/CapsuleShooterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UEndMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton->OnClicked.AddDynamic(this, &ThisClass::ExitButtonPressed);
	ReplayButton->OnClicked.AddDynamic(this, &ThisClass::ReplayButtonPressed);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TEXT("End menu set up"));
}

void UEndMenu::ExitButtonPressed()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
}

void UEndMenu::ReplayButtonPressed()
{
	Cast<UCapsuleShooterGameInstance>(UGameplayStatics::GetGameInstance(this))->ResetGame();
}

void UEndMenu::SetScore(FText Score)
{
	if (ScoreText)
	{
		ScoreText->SetText(Score);
	}
}
