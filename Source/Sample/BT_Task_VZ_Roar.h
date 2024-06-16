// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ViolinZombieController.h"
#include "ViolinZombie.h"
#include "MyCharacter.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BT_Task_VZ_Roar.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API UBT_Task_VZ_Roar : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Task_VZ_Roar();

	virtual EBTNodeResult::Type ExecuteTask
	(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void GetOwnerInfo(UBehaviorTreeComponent& OwnerComp);

	AViolinZombieController* VZController;
	UBlackboardComponent* VZ_BB;
	AViolinZombie* OwnerZombie;

	void Roar();

	void CoolDown();
};
