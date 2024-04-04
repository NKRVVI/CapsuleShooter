// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_WithinDistanceFromTarget.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UBTD_WithinDistanceFromTarget : public UBTDecorator
{
	GENERATED_BODY()

	UBTD_WithinDistanceFromTarget();
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MaximumDistanceKey;
	
};