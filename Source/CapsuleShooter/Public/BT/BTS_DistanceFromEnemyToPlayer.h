// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_DistanceFromEnemyToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UBTS_DistanceFromEnemyToPlayer : public UBTService
{
	GENERATED_BODY()

	UBTS_DistanceFromEnemyToPlayer();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToPlayerKey;
	
};
