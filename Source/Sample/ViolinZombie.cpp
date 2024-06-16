// Fill out your copyright notice in the Description page of Project Settings.


#include "ViolinZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

// ToDo List
// 바이올린 좀비는 필드에 플레이어가 들어오는 것으로 플레이어를 인식한다 [v]
// 플레이어를 발견하면 크게 소리쳐 주변 좀비들에게 플레이어의 위치를 알린다
// 직후 연주를 통해 적들을 더욱 공격적이게 강화한다 [v]
// 연주를 마치고나면 주변의 상황을 파악한다 [v]
// 여전히 플레이어가 있다면?
// -> 플레이어랑 즉시 싸우게 된다 [v]
// 만일 플레이어가 없으면? [v]
// -> 마지막으로 플레이어가 목격된 곳으로 가본다 [v]

// 다리 형태 -> 높은 점프를 뛰기에 유리해 보임
// 연주 패턴 이후 혹은 발견 직후 고지대로 점프하는 기믹
// 혹은 근접 전투를 펄쩍 뛰어서 접근해오는 방식 구상

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
	// 범위 밖으로 플레이어가 나가면?
	if (OtherActor->ActorHasTag("Player"))
	{
		// 3초 후 놓침
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

	// 포효
	for (int i = 0; i < InFieldEnemys.Num(); i++)
	{
		AEnemyController* TargetController =
			Cast<AEnemyController>(InFieldEnemys[i]->GetController());

		if (TargetController)
		{
			// VZ가 가진 플레이어 정보 전달
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
	// 연주
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
