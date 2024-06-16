// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Find_Player.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_Find_Player::UBTService_Find_Player()
{
	NodeName = (TEXT("Finding Player"));
}

void UBTService_Find_Player::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player)
	{
		if (OwnerComp.GetAIOwner())
		{
			OwnerComp.GetBlackboardComponent()->
				SetValueAsObject(GetSelectedBlackboardKey(), Player);
		}

		else
		{
			UE_LOG(LogTemp, Error, TEXT("Find Player OwnerComp.GetAIOwner Error"));
			return;
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Find Player Player Error"));
		return;
	}
}
