// Fill out your copyright notice in the Description page of Project Settings.


#include "TankZombie.h"
#include "Projectile.h"

// ��ũ ����
/*
	1. ���������� ��� [v]
	2. �÷��̾��� ��ġ�� ����ؼ� �˰� ���� ���� [v]
	3. �÷��̾� �ֺ��� ���� �Ÿ����� �޷��´� [v]
	4. �Ÿ��� ���� ��� �ִ� ��ȯ�� �÷��̾ ���� ������ [v]
	 - ���� �÷��̾�� ��� ������ ����� ���°� �ȴ� [v]
	5. ���� ���� ��ȿ [v]
	6. ��ȿ�� ������ �÷��̾ ���������� �߰��ؿ��� ���� [v]
	7. ������ ���� �÷��̾ ������ ���� ������ �ֵθ��� ���ߴ��� �����
	���ư���. [v]
*/

ATankZombie::ATankZombie()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>
		(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(),
		TEXT("Projectile Point"));

	// ��ȯ ���ط�
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