// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsPlayerWithinRange.generated.h"

UCLASS()
class CAPSULESHOOTER_API UBTD_IsPlayerWithinRange : public UBTDecorator
{
	GENERATED_BODY()
	
	UBTD_IsPlayerWithinRange();
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToPlayerKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector RangeKey;
};
