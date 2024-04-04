// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTD_AttackCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_AttackCooldown::UBTD_AttackCooldown()
{
	NodeName = "Attack Cooldown";
}

void UBTD_AttackCooldown::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	Super::OnNodeActivation(SearchData);
	CoolDownTime = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AttackCooldownKey.SelectedKeyName);
}
