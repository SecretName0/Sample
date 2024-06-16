// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ViolinZombie.h"
#include "MyCharacter.h"
#include "BT_Task_VZ_JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API UBT_Task_VZ_JumpAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBT_Task_VZ_JumpAttack();

	virtual EBTNodeResult::Type ExecuteTask
	(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	void OverJumping(AViolinZombie* Owner, AMyCharacter* TargetPlayer);
	void Jumping(AViolinZombie* Owner, AMyCharacter* TargetPlayer
	,float AfterJumpDistance);

	FVector CalculatingJumpVelocity(FVector VZLoca, FVector TargetLoca,
		float LaunchAngle);
	FVector CalculatingAdjustJumpDistance(FVector PlayerLocation,
		FVector OwnerLocation, float DesiredDistance);

	void GetOwnerInfo(UBehaviorTreeComponent& OwnerComp);

	AViolinZombieController* VZController;
	UBlackboardComponent* VZ_BB;
	AViolinZombie* OwnerZombie;

	void CoolDown();
};
