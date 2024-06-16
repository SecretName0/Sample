// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Service_UpdatePlayerInfo.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBT_Service_UpdatePlayerInfo::UBT_Service_UpdatePlayerInfo()
{
	NodeName = (TEXT("Update Player Data"));
}

void UBT_Service_UpdatePlayerInfo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (OwnerComp.GetAIOwner()->LineOfSightTo(Player))
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsObject(GetSelectedBlackboardKey(), Player);
	}

	else
	{
		// 시야에 들지 않았음
		Timer += DeltaSeconds;

		if(Timer >= 3)
		{
			Timer = 0;

			OwnerComp.GetBlackboardComponent()->
				ClearValue(GetSelectedBlackboardKey());
		}
	}
}
