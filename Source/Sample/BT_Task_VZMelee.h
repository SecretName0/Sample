// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Task_VZMelee.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API UBT_Task_VZMelee : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Task_VZMelee();

	virtual EBTNodeResult::Type ExecuteTask
	(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
