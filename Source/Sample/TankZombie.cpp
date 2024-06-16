// Fill out your copyright notice in the Description page of Project Settings.


#include "TankZombie.h"
#include "Projectile.h"

// 탱크 좀비
/*
	1. 보스격으로 취급 [v]
	2. 플레이어의 위치를 계속해서 알고 있을 것임 [v]
	3. 플레이어 주변의 일정 거리까지 달려온다 [v]
	4. 거리에 들어서면 들고 있던 포환을 플레이어를 향해 던진다 [v]
	 - 맞은 플레이어는 잠깐 쓰러져 무방비 상태가 된다 [v]
	5. 던진 직후 포효 [v]
	6. 포효가 끝나면 플레이어를 본격적으로 추격해오기 시작 [v]
	7. 사정권 내로 플레이어가 들어오면 근접 공격을 휘두르고 적중당한 대상은
	날아간다. [v]
*/

ATankZombie::ATankZombie()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>
		(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(),
		TEXT("Projectile Point"));

	// 포환 피해량
	Damage = 150;

	MeleeDamage = 100;

	Max_HP = 5000;
}

void ATankZombie::BeginPlay()
{
	Super::BeginPlay();

	HP = Max_HP;
}

void ATankZombie::Roaring()
{
	PlayAnimMontage(RoarMontage);
}

void ATankZombie::TankThrowAttack()
{
	PlayAttackAnim();

	FVector Loca = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rota = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile =
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Loca, Rota);

	Projectile->SetOwner(this);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this,
		&ATankZombie::Roaring, 0.25f, false);
}