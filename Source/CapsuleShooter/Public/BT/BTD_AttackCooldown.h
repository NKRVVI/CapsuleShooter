// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "BTD_AttackCooldown.generated.h"

/**
 * 
 */
UCLASS()
class CAPSULESHOOTER_API UBTD_AttackCooldown : public UBTDecorator_Cooldown
{
	GENERATED_BODY()
	
	UBTD_AttackCooldown();

	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData);

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackCooldownKey;
};