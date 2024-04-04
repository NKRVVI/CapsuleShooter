// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndMenu.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UEndMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ExitButtonPressed();

	UFUNCTION()
	void ReplayButtonPressed();

	void SetScore(FText Score);

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;
	
};
