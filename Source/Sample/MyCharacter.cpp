// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "MyGun.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeArea = CreateDefaultSubobject<UBoxComponent>
		(TEXT("Melee Attack Area"));
	MeleeArea->SetupAttachment(GetMesh(),
		TEXT("Melee Attack Area"));

}

void AMyCharacter::Dead()
{
	// 사망 애니메이션(은 애님 인스턴스에서)
	IsDead = true;

	// 에너미라면 일단 사망 체크 후, 바이올린 좀비에 의해 강화되었나 체크
	// 안됐다면 여기서 파괴처리
	// 됐다면 바이올린 좀비가 처리
	if (IsEnemy)
	{
		if (!InVZField)
		{
			Destroy();
		}
	}
	
	// 플레이어는 따로 처리

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Gun = GetWorld()->SpawnActor<AMyGun>(GunClass);

	MeleeArea->OnComponentBeginOverlap.AddDynamic(this,
		&AMyCharacter::OnMeleeAreaOverlapBegin);

	CanActions = true;

	if (ActorHasTag("Enemy"))
	{
		IsEnemy = true;
	}

	SetShootSetting();
	DisableMeleeTrigger();
	InitAttackState();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookPitchRotate"), this, &AMyCharacter::RotatePitch);
	PlayerInputComponent->BindAxis(TEXT("LookYawRotate"), this, &AMyCharacter::RotateYaw);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("Melee"), IE_Pressed, this, &AMyCharacter::MeleeAttack);
}

void AMyCharacter::KnockBack(ACharacter* HitCharacter, float KnockBackPower)
{
	FVector AtkDir = HitCharacter->GetActorLocation() - GetActorLocation();
	AtkDir.Normalize();
	FVector KnockBackDistance = AtkDir * KnockBackPower;

	HitCharacter->LaunchCharacter(KnockBackDistance, false, false);
}

void AMyCharacter::AIMeleeAttack()
{
	PlayAnimMontage(MeleeMontage);
	MeleeArea->SetGenerateOverlapEvents(true);

	FTimerHandle DisableTriggerTimer;
	GetWorldTimerManager().SetTimer(DisableTriggerTimer, this,
		&AMyCharacter::DisableMeleeTrigger, 0.2f, false);
	//UE_LOG(LogTemp, Warning, TEXT("AI Melee Attack!!"));
}

void AMyCharacter::MoveForward(float Value)
{
	if(CanActions)
	{
		FVector FinalScale = GetActorForwardVector() * Value;
		AddMovementInput(FinalScale);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if(CanActions)
	{
		FVector FinalScale = GetActorRightVector() * Value;
		AddMovementInput(FinalScale);
	}
}

void AMyCharacter::RotatePitch(float Value)
{
	if(CanActions)
		AddControllerPitchInput(Value * MouseSensitivity * GetWorld()->DeltaTimeSeconds);
}

void AMyCharacter::RotateYaw(float Value)
{
	if(CanActions)
		AddControllerYawInput(Value * MouseSensitivity * GetWorld()->DeltaTimeSeconds);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//AMyCharacter* Attacker = Cast<AMyCharacter>(DamageCauser);

	HP -= DamageToApply;

	// 약점 공격 or 교체 가능 부위 공격 판별
	if (DamageCauser->ActorHasTag("Player") || DamageCauser->ActorHasTag("Enemy"))
	{
		AMyCharacter* Attacker = Cast<AMyCharacter>(DamageCauser);

		// 약점 공격에 성공했는가?
		if (Attacker->HitWeakPoint && BreakPointHP > 0)
		{
			BreakPartsCalculating(1);
		}

		// 교체가 가능한 부위에 명중 시켰는가?
		else if (Attacker->HitCanChangePoint && ChangePointHP > 0)
		{
			ChangingPartsCalculating(1);
		}
	}

	// 발사체에 피해를 입은 경우
	else if (DamageCauser->ActorHasTag("Projectile"))
	{
		IsDown = true;
		CanActions = false;

		FTimerHandle StandUpTimer;
		GetWorldTimerManager().SetTimer(StandUpTimer, this,
			&AMyCharacter::StandUp, 3, false);
		
		//UE_LOG(LogTemp, Error, TEXT("Projectile Damage"));
	}

	if (HP > 0)
	{
		// Hit Reaction
		// PlayAnimMontage(HitAnim);
	}

	if (HP <= 0)
	{
		if (this->ActorHasTag("Enemy"))
		{
			// Death Anim
		}
	}

	/*UE_LOG(LogTemp, Warning, TEXT("--- Hit Target Data ---"));
	UE_LOG(LogTemp, Log, TEXT("MaxHP: %f"), Max_HP);
	UE_LOG(LogTemp, Log, TEXT("NowHP: %f"), HP);
	UE_LOG(LogTemp, Log, TEXT("Damage: %f"), DamageToApply);*/

	return DamageToApply;
}

void AMyCharacter::Fire()
{
	if(CanFire && CanActions)
	{
		NowAttack();

		AController* ShooterController;
		FVector CurrentLocation;
		FRotator CurrentRotation;
		// ------------------------------- //
		ShooterController = this->GetController();

		if (ShooterController == nullptr)
			return;

		ShooterController->GetPlayerViewPoint(CurrentLocation, CurrentRotation);

		FVector ShotDirection = -CurrentRotation.Vector();

		FHitResult HitResult;

		PlayAttackAnim();

		FVector InRange = CurrentLocation + CurrentRotation.Vector() * MaxRange;

		bool bSuccessHit = GetWorld()->LineTraceSingleByChannel(HitResult,
			CurrentLocation, InRange, ECC_GameTraceChannel1, Params);

		if (bSuccessHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, HitResult,
					ShotDirection, nullptr);

				GiveDamage = Damage;

				// 약점 명중
				if (HitResult.BoneName == BreakPointName)
				{
					GiveDamage *= 2;
					HitWeakPoint = true;
					HitCanChangePoint = false;

					//UE_LOG(LogTemp, Warning, TEXT("<<< HeadShot >>>"));
					//UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), GiveDamage);

					HitActor->TakeDamage(GiveDamage, DamageEvent, ShooterController,
						this);
				}

				// 교체 가능 부위 명중
				else if (HitResult.BoneName == ChangePointName)
				{
					HitWeakPoint = false;
					HitCanChangePoint = true;

					//UE_LOG(LogTemp, Warning, TEXT("<<< HeadShot >>>"));
					//UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), GiveDamage);

					HitActor->TakeDamage(GiveDamage, DamageEvent, ShooterController,
						this);
				}

				// 일반 명중
				else
				{
					HitWeakPoint = false;
					HitCanChangePoint = false;

					//UE_LOG(LogTemp, Display, TEXT("--- Normal ---"));
					//UE_LOG(LogTemp, Display, TEXT("Damage: %f"), GiveDamage);

					HitActor->TakeDamage(GiveDamage, DamageEvent, ShooterController,
						this);
				}

				UE_LOG(LogTemp, Error, TEXT("Hit Point: %s"), *HitResult.BoneName.ToString());
			}
		}

		StateInitTimer(FireDelay);
	}
}

void AMyCharacter::MeleeAttack()
{
	if(CanMelee && CanActions)
	{
		NowAttack();

		PlayAnimMontage(MeleeMontage);
		MeleeArea->SetGenerateOverlapEvents(true);

		FTimerHandle DisableTriggerTimer;
		GetWorldTimerManager().SetTimer(DisableTriggerTimer, this,
			&AMyCharacter::DisableMeleeTrigger, 0.2f, false);

		StateInitTimer(MeleeDelay);
	}
}

void AMyCharacter::InitAttackState()
{
	CanFire = true;
	CanMelee = true;
}

void AMyCharacter::NowAttack()
{
	CanFire = false;
	CanMelee = false;
}

void AMyCharacter::StateInitTimer(float WaitTime)
{
	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, this,
		&AMyCharacter::InitAttackState, WaitTime, false);
}

void AMyCharacter::OnMeleeAreaOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Error, TEXT("Melee call"));
	if (OtherActor && OtherActor != this)
	{
		//UE_LOG(LogTemp, Error, TEXT("Actor Overlap"));

		if (OtherActor->ActorHasTag("Player") && ActorHasTag("Tank"))
		{
			//UE_LOG(LogTemp, Error, TEXT("Player Overlap"));
			ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
			KnockBack(HitCharacter, 1000);
		}
		UGameplayStatics::ApplyDamage(OtherActor, MeleeDamage,
			GetController(), this, nullptr);
	}
}

void AMyCharacter::DisableMeleeTrigger()
{
	MeleeArea->SetGenerateOverlapEvents(false);
	//UE_LOG(LogTemp, Error, TEXT("Melee Disable"));
}

void AMyCharacter::SetShootSetting()
{
	Params.AddIgnoredActor(this);
}

void AMyCharacter::PlayAttackAnim()
{
	PlayAnimMontage(FireMontage);
}

void AMyCharacter::BreakPartsCalculating(int BreakPointDamage)
{
	BreakPointHP -= BreakPointDamage;

	if (BreakPointHP <= 0)
	{
		BreakWeakPoint = true;

		if (BreakWeakPoint)
		{
			BreakParts();

			// IsDead
		}
	}
}

void AMyCharacter::BreakParts()
{
	GetMesh()->HideBoneByName(BreakPointName, EPhysBodyOp::PBO_Term);
	// Term: 숨길 부위의 하위 본들도 숨김
	GetMesh()->SetAllBodiesBelowSimulatePhysics(BreakPointName, false);
}

void AMyCharacter::ChangingPartsCalculating(int PointDamage)
{
	ChangePointHP -= PointDamage;

	if (ChangePointHP <= 0)
	{
		BreakChangePoint = true;

		if (BreakChangePoint)
		{
			ChangingParts();
		}
	}
}

void AMyCharacter::ChangingParts()
{
	GetMesh()->HideBoneByName(ChangePointName, EPhysBodyOp::PBO_None);

	Gun->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

void AMyCharacter::StandUp()
{
	IsDown = false;

	FTimerHandle ActionTimer;
	GetWorldTimerManager().SetTimer(ActionTimer, this,
		&AMyCharacter::CanAction, 1, false);
}

void AMyCharacter::CanAction()
{
	CanActions = true;
}

