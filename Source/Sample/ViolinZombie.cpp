// Fill out your copyright notice in the Description page of Project Settings.


#include "ViolinZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

// ToDo List
// ���̿ø� ����� �ʵ忡 �÷��̾ ������ ������ �÷��̾ �ν��Ѵ� [v]
// �÷��̾ �߰��ϸ� ũ�� �Ҹ��� �ֺ� ����鿡�� �÷��̾��� ��ġ�� �˸���
// ���� ���ָ� ���� ������ ���� �������̰� ��ȭ�Ѵ� [v]
// ���ָ� ��ġ���� �ֺ��� ��Ȳ�� �ľ��Ѵ� [v]
// ������ �÷��̾ �ִٸ�?
// -> �÷��̾�� ��� �ο�� �ȴ� [v]
// ���� �÷��̾ ������? [v]
// -> ���������� �÷��̾ ��ݵ� ������ ������ [v]

// �ٸ� ���� -> ���� ������ �ٱ⿡ ������ ����
// ���� ���� ���� Ȥ�� �߰� ���� ������� �����ϴ� ���
// Ȥ�� ���� ������ ��½ �پ �����ؿ��� ��� ����

AViolinZombie::AViolinZombie()
{
	SoundField = CreateDefaultSubobject<USphereComponent>(TEXT("Sound Sphere Collision"));
	SoundField->SetSphereRadius(1000);
	SoundField->SetCollisionProfileName(TEXT("OverlapAll"));
	SoundField->SetupAttachment(GetMesh());
}

void AViolinZombie::BeginPlay()
{
	Super::BeginPlay();

	VZController = Cast<AViolinZombieController>(GetController());

	SoundField->OnComponentBeginOverlap.AddDynamic(this,
		&AViolinZombie::OnSphereOverlapBegin);

	SoundField->OnComponentEndOverlap.AddDynamic(this,
        &AViolinZombie::OnSphereEndOverlap);
}

void AViolinZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckCombatState();

	if (!NowCombat && VZController->BlackBoardComponent
		->GetValueAsBool("Player"))
	{
		InitArray();
	}

	///*
	DrawDebugSphere(GetWorld(), SoundField->GetComponentLocation(),
		SoundField->GetScaledSphereRadius(), 24, FColor::Red,
		false, 0.1f);
	//*/
}
void AViolinZombie::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Error, TEXT("call"));

	if (OtherActor->ActorHasTag("Player"))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(MissPlayer))
		{
			GetWorld()->GetTimerManager().ClearTimer(MissPlayer);
			UE_LOG(LogTemp, Warning, TEXT("MissTimer Handle stopped!"));
		}

		VZController->FindPlayer();

		if (VZController->BlackBoardComponent->GetValueAsObject("Player"))
		{
			GetInFieldEnemys();

			if (InFieldEnemys.Num() < 1)
			{
				VZController->BlackBoardComponent
					->SetValueAsBool("NowGroup", false);
			}

			else
			{
				VZController->BlackBoardComponent
					->SetValueAsBool("NowGroup", true);
			}
		}
	}
}

void AViolinZombie::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp,class AActor* OtherActor, class UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	// ���� ������ �÷��̾ ������?
	if (OtherActor->ActorHasTag("Player"))
	{
		// 3�� �� ��ħ
		GetWorld()->GetTimerManager().SetTimer(MissPlayer, this,
			&AViolinZombie::MissedPlayer, 3, false);
	}
}

void AViolinZombie::SetMoveSpeed()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	if (NowCombat)
		MoveComp->MaxWalkSpeed = CombatSpeed;

	else
		MoveComp->MaxWalkSpeed = NonCombatSpeed;
}

void AViolinZombie::RoarEffect()
{
	PlayAnimMontage(RoarMontage);

	// ��ȿ
	for (int i = 0; i < InFieldEnemys.Num(); i++)
	{
		AEnemyController* TargetController =
			Cast<AEnemyController>(InFieldEnemys[i]->GetController());

		if (TargetController)
		{
			// VZ�� ���� �÷��̾� ���� ����
			TargetController->GetSign = true;
		}

		else if (!TargetController)
		{
			UE_LOG(LogTemp, Error, TEXT("NO Cont"));
		}
	}
}

void AViolinZombie::PerformanceEffect()
{
	PlayAnimMontage(UltimateMontage);
	// ����
	for (int i = 0; i < InFieldEnemys.Num(); i++)
	{
		InFieldEnemys[i]->Damage += (Damage * 0.1f);
		InFieldEnemys[i]->GetCharacterMovement()->MaxWalkSpeed *= 1.1f;
	}
}

void AViolinZombie::InitArray()
{
	//UE_LOG(LogTemp, Error, TEXT("Reset"));

	if (!NowCombat)
		InFieldEnemys.Empty();
}

void AViolinZombie::CheckCombatState()
{
	if (VZController->BlackBoardComponent->GetValueAsObject("Player"))
		NowCombat = true;

	else
		NowCombat = false;

	SetMoveSpeed();
}

void AViolinZombie::GetInFieldEnemys()
{
	TArray<AActor*> OverlappingTargets;
	SoundField->GetOverlappingActors(OverlappingTargets);

	for (AActor* MyCharacter : OverlappingTargets)
	{
		if (MyCharacter->ActorHasTag("Enemy"))
		{
			AMyCharacter* IncludeChara = Cast<AMyCharacter>(MyCharacter);
			IncludeChara->InVZField = true;

			UE_LOG(LogTemp, Display, TEXT("%s"), *IncludeChara->GetName());

			InFieldEnemys.Add(IncludeChara);
		}
	}
}

void AViolinZombie::CheckDeadEnemy()
{
	for (AMyCharacter* DeadActor : InFieldEnemys)
	{
		if (DeadActor->IsDead)
		{
			InFieldEnemys.Remove(DeadActor);

			DeadActor->Destroy();
		}
	}
}

void AViolinZombie::MissedPlayer()
{
	VZController->BlackBoardComponent->ClearValue("Player");
}
