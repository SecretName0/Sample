// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.generated.h"

class AMyGun;

UCLASS()
class SAMPLE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	void PlayAttackAnim();
	// ----- Damage ----- //
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Damage = 50;

	float MeleeDamage = 30;

	float GiveDamage;

	UPROPERTY(EditAnywhere, Category = "Life")
	float Max_HP;
	UPROPERTY(EditAnywhere, Category = "Life")
	float HP;

	// ----- Breaking ----- //
	UPROPERTY(EditAnywhere, Category = "Break Parts")
	int BreakPointHP = 3;

	FName BreakPointName = TEXT("head");

	UPROPERTY(VisibleAnywhere, Category = "Break Parts")
	bool HitWeakPoint;

	UPROPERTY(VisibleAnywhere, Category = "Break Parts")
	bool BreakWeakPoint;

	void BreakPartsCalculating(int BreakPointDamage);

	// ----- Change Parts ----- //
	UPROPERTY(EditAnywhere, Category = "Change Parts")
	int ChangePointHP = 3;

	FName ChangePointName = TEXT("weapon_r"); // 현재는 미니언 메시에만 해당됨

	UPROPERTY(VisibleAnywhere, Category = "Change Parts")
	bool HitCanChangePoint;

	UPROPERTY(VisibleAnywhere, Category = "Break Parts")
	bool BreakChangePoint;

	void ChangingPartsCalculating(int PointDamage);

	// ----- Mouse ----- //
	UPROPERTY(EditAnywhere, Category = "Option Setting")
	float MouseSensitivity = 15;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	// ----- Melee ----- //
	UFUNCTION()
	void OnMeleeAreaOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	void DisableMeleeTrigger();

	// ----- State ----- //
	bool IsDead;

	bool CanFire;
	float FireDelay = 0.6f;
	bool CanMelee;
	float MeleeDelay = 1;

	UPROPERTY(BlueprintReadWrite)
	bool IsDown;

	UPROPERTY(BlueprintReadWrite)
	bool CanActions;

	// ----- etc ----- //
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMyGun> GunClass;

	AMyGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjectileClass;

	void KnockBack(ACharacter* HitCharacter, float KnockBackPower);

	// ----- AI ----- //
	void AIMeleeAttack();

	bool InVZField;

	void Dead();

	bool IsEnemy;

	// ----- Anim Montage ----- //
	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* MeleeMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* RoarMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* UltimateMontage;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FCollisionQueryParams Params;

	float MaxRange = 10000;

	// ----- Components ----- //
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* MeleeArea;

	

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotatePitch(float Value);
	void RotateYaw(float Value);

	void Fire();
	void MeleeAttack();
	void InitAttackState();
	void NowAttack();
	void StateInitTimer(float WaitTime);

	void SetShootSetting();

	void BreakParts();
	void ChangingParts();

	void StandUp();
	void CanAction();
};
