// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyTree != nullptr)
	{
		RunBehaviorTree(EnemyTree);

		UseBlackboard(BlackBoardData, BlackBoardComponent);
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		// �÷��̾ �������� �˷���
	}
}

void AEnemyController::InitSign()
{
	GetSign = false;
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
