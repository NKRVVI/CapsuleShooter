// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_StopMovement.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UBTD_StopMovement : public UBTDecorator
{
	GENERATED_BODY()

	UBTD_StopMovement();
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector StopMovementKey;
	
};
