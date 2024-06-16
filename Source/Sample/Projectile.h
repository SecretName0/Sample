// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyCharacter.h"
#include "Projectile.generated.h"

UCLASS()
class SAMPLE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* PMC;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	void GetTargetData(AActor* Actor);

	AMyCharacter* HitTarget;

	virtual void ApplyDamage(AActor* DamagedActor,
		float BaseDamage, AController* EventInstigator,
		AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass);
};
