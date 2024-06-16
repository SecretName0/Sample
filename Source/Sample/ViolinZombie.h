// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"
#include "EnemyController.h"
#include "ViolinZombieController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ViolinZombie.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AViolinZombie : public AMyCharacter
{
	GENERATED_BODY()
	
public:
	AViolinZombie();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Component")
	USphereComponent* SoundField;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AMyCharacter*> InFieldEnemys;

	void GetInFieldEnemys();

	void RoarEffect();
	void PerformanceEffect();

	bool NowCombat;
	bool CanPerform;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	AViolinZombieController* VZController;

	float NonCombatSpeed = 300.f;
	float CombatSpeed = 1200.f;

	void SetMoveSpeed();

	void InitArray();
	void CheckCombatState();
	void CheckDeadEnemy();

	FTimerHandle MissPlayer;
	void MissedPlayer();
};
