// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "Tank_Controller.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ATank_Controller : public AEnemyController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	float CertainThrowDistance = 1000; // 최소 거리


private:
};
