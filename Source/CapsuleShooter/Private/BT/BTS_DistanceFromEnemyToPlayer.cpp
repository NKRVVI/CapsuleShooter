// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTS_DistanceFromEnemyToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_DistanceFromEnemyToPlayer::UBTS_DistanceFromEnemyToPlayer()
{
	NodeName = "Update Distance To Player";
}

void UBTS_DistanceFromEnemyToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerKey.SelectedKeyName));

	if (Self && Player)
	{
		float Distance = FVector::Dist(Self->GetActorLocation(), Player->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToPlayerKey.SelectedKeyName, Distance);
	}
}
