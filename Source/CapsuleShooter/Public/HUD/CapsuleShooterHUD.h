// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CapsuleShooterHUD.generated.h"

class UHealthBarWidget;
class ULevelHeaderWidget;
class UOpeningMenu;
class UEndMenu;

UCLASS()
class CAPSULESHOOTER_API ACapsuleShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	void AddHealthBarWidgetToViewport();
	void AddLevelHeaderWidgetToViewport();
	void AddEndMenuWidgetToViewport();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UHealthBarWidget* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;
	
	UPROPERTY()
	ULevelHeaderWidget* LevelHeaderWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULevelHeaderWidget> LevelHeaderWidgetClass;

	UPROPERTY()
	UOpeningMenu* OpeningMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOpeningMenu> OpeningMenuClass;

	UPROPERTY()
	UEndMenu* EndMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEndMenu> EndMenuClass;

public:
	FORCEINLINE UHealthBarWidget* GetHealthBarWidget() const { return HealthBarWidget; }
	FORCEINLINE ULevelHeaderWidget* GetLevelHeaderWidget() const { return LevelHeaderWidget; }
	FORCEINLINE UEndMenu* GetEndMenuWidget() const { return EndMenuWidget; }
};
