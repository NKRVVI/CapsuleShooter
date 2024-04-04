// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTD_IsPlayerWithinRange.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsPlayerWithinRange::UBTD_IsPlayerWithinRange()
{
	NodeName = "IsPlayerWithinRange";
}

bool UBTD_IsPlayerWithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float DistanceToPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DistanceToPlayerKey.SelectedKeyName);
	float Range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(RangeKey.SelectedKeyName);

	return DistanceToPlayer <= Range;
}