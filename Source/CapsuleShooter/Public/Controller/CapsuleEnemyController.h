// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CapsuleEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API ACapsuleEnemyController : public AAIController
{
	GENERATED_BODY()
	
	virtual void OnPossess(APawn* PossessedPawn) override;

public:
	class AEnemy* PossessedEnemy;

protected:
	UFUNCTION()
	void StopEnemyMovement();
};
