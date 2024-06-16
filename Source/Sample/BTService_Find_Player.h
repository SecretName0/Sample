// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Find_Player.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API UBTService_Find_Player : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_Find_Player();

protected:
	virtual void TickNode
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;
};
