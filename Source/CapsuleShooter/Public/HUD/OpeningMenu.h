// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningMenu.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UOpeningMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BeginButtonPressed();

	UFUNCTION()
	void ExitButtonPressed();
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* BeginButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};
