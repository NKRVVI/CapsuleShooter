// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTT_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//void Attack(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackSpeedKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector IsAttackingKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DamageKey;

	class AEnemy* Self;
	class ACapsulePlayer* Target;
	float AttackSpeed;
	FVector SelfLocation;
	FVector TargetLocation;
	float Damage;

	bool TowardsTarget = true;

	FTimerHandle AttackTimer;

	int32 NumLoops = 0;
};
