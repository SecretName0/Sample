// Fill out your copyright notice in the Description page of Project Settings.


#include "ViolinZombieController.h"
#include "ViolinZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

	// Violin Zombie에서 참조중
void AViolinZombieController::FindPlayer()
{
	// PC o / S x (최초 목격)
	if (LineOfSightTo(Player) && !BlackBoardComponent->GetValueAsBool("SoundSearch"))
	{
		SetSoundLocation(true);
		BlackBoardComponent->SetValueAsObject("Player", Player);
	}

	// PC o / s o
	else if (LineOfSightTo(Player) && BlackBoardComponent->GetValueAsBool("SoundSearch"))
	{
		SetSoundLocation(true);
	}

	// PC x / S o
	else if (!LineOfSightTo(Player))
	{
		SetSoundLocation(true);
	}
	// PC x / S x || PC o / S x
	else
	{
		SetSoundLocation(false);
		BlackBoardComponent->ClearValue("Player");
	}
}

void AViolinZombieController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AViolinZombieController::BeginPlay()
{
	Super::BeginPlay();

	OwnerVZ = Cast<AViolinZombie>(GetPawn());
	// 연주를 하지 않은 상태기 때문에 할 수 있다 체크
	BlackBoardComponent->SetValueAsBool("CanPerform", true);
	BlackBoardComponent->SetValueAsBool("CanRoar", true);

	SetOriginLocation();
}

void AViolinZombieController::SetOriginLocation()
{
	BlackBoardComponent->SetValueAsVector("SelfOriginLocation",
		OwnerVZ->GetActorLocation());
}

void AViolinZombieController::SetSoundLocation(bool SoundCatch)
{
	BlackBoardComponent->SetValueAsBool("SoundSearch", SoundCatch);

	if(SoundCatch)
	{
		BlackBoardComponent->SetValueAsVector("SoundLocation",
			Player->GetActorLocation());
	}
}
