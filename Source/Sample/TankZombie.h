// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "TankZombie.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API ATankZombie : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	ATankZombie();

	void TankThrowAttack();

	

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	void Roaring();
};
