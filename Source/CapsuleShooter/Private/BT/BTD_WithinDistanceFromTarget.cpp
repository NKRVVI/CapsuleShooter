// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTD_WithinDistanceFromTarget.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_WithinDistanceFromTarget::UBTD_WithinDistanceFromTarget()
{
	NodeName = "WithinDistanceFromTarget";
}

bool UBTD_WithinDistanceFromTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FVector SelfLocation = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName))->GetActorLocation();
	FVector TargetLocation = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName))->GetActorLocation();
	float MaximumDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MaximumDistanceKey.SelectedKeyName);

	return (SelfLocation - TargetLocation).Size() <= MaximumDistance;
}

