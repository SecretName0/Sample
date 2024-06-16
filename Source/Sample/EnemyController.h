// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBlackboardData* BlackBoardData;

	UPROPERTY(EditAnywhere)
	UBlackboardComponent* BlackBoardComponent;

	APawn* Player;

	UPROPERTY(VisibleAnywhere)
	bool GetSign;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyTree;

	void InitSign();
};
