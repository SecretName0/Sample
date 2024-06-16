// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "MyCharacter.h"
#include "TankZombie.h"
#include "Kismet/GameplayStatics.h"
#include "Tank_Controller.h"
#include "Engine/DamageEvents.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("Projectile Mesh"));

	RootComponent = ProjectileMesh;

	PMC = CreateDefaultSubobject<UProjectileMovementComponent>
		(TEXT("Projectile Movement Component"));

	PMC->MaxSpeed = 5000; // 최대 속도
	PMC->InitialSpeed = 5000; // 초기 발사 속도
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetTargetData(OtherActor);
	//UE_LOG(LogTemp, Display, TEXT("HitTarget: %s"),
		//*HitTarget->GetName());

	if (HitTarget)
	{
		if (HitTarget->ActorHasTag("Player") || HitTarget->ActorHasTag("Enemy"))
		{
			ATankZombie* ProjectileOwner = Cast<ATankZombie>(GetOwner());

			if(ProjectileOwner)
			{
				//UE_LOG(LogTemp, Display, TEXT("%s"), 
					//*ProjectileOwner->GetActorNameOrLabel());

				ATank_Controller* AttackerController =
					Cast<ATank_Controller>(ProjectileOwner->
						GetInstigatorController());

				if (AttackerController)
				{
					//UE_LOG(LogTemp, Display, TEXT("%s"),
						//*AttackerController->GetName());

					ApplyDamage(HitTarget, ProjectileOwner->Damage,
						AttackerController, this, nullptr);
				}
			}
		}
	}

	Destroy();
}

void AProjectile::GetTargetData(AActor* Actor)
{
	HitTarget = Cast<AMyCharacter>(Actor);
}

void AProjectile::ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	FHitResult HitResult;
	FVector ThrowDir = DamageCauser->GetActorRotation().Vector();
	FPointDamageEvent DamageEvent(BaseDamage, HitResult, ThrowDir, nullptr);

	if (HitTarget->ActorHasTag("Player"))
	{
		// 교체 가능 포인트 명중
		if (HitResult.BoneName == HitTarget->BreakPointName || HitResult.BoneName == HitTarget->ChangePointName)
		{
			UE_LOG(LogTemp, Display, TEXT("Guard"));

			HitTarget->ChangingPartsCalculating(2);

			HitTarget->TakeDamage(BaseDamage *0.25f, DamageEvent, EventInstigator,
				this);
		}

		else
		{
			UE_LOG(LogTemp, Display, TEXT("Projectile_Normal Damage"));

			HitTarget->TakeDamage(BaseDamage, DamageEvent, EventInstigator,
				this);
		}
	}

	else if (HitTarget->ActorHasTag("Enemy"))
	{
		HitTarget->TakeDamage(BaseDamage * 0.1f, DamageEvent,
			EventInstigator, this);
	}
}

