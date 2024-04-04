// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTD_StopMovement.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_StopMovement::UBTD_StopMovement()
{
	NodeName = "StopEnemyMovement";
}

bool UBTD_StopMovement::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool StopEnemyMovement = OwnerComp.GetBlackboardComponent()->GetValueAsBool(StopMovementKey.SelectedKeyName);
	return StopEnemyMovement;
}
