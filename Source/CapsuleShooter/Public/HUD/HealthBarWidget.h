// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
