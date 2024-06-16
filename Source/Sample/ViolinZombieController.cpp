// Fill out your copyright notice in the Description page of Project Settings.


#include "ViolinZombieController.h"
#include "ViolinZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

	// Violin Zombie���� ������
void AViolinZombieController::FindPlayer()
{
	// PC o / S x (���� ���)
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
	// ���ָ� ���� ���� ���±� ������ �� �� �ִ� üũ
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
