// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetSign.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyController.h"
#include "MyCharacter.h"
#include "AIController.h"

UBTService_GetSign::UBTService_GetSign()
{
	NodeName = (TEXT("If Get Sign?"));
}

void UBTService_GetSign::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AEnemyController* OwnerUnit = Cast<AEnemyController>(OwnerComp.GetOwner());

	if (OwnerUnit)
	{
		AEnemyController* OwnerCont = Cast<AEnemyController>
			(OwnerUnit->GetInstigatorController());

		if (OwnerCont && OwnerCont->GetSign && Player)
		{
			if (OwnerComp.GetAIOwner())
			{
				/*if (OwnerComp.GetAIOwner()->LineOfSightTo(Player))
				{
					OwnerComp.GetBlackboardComponent()->
						SetValueAsObject(GetSelectedBlackboardKey(), Player);
				}

				else
				{
					OwnerComp.GetBlackboardComponent()->
						ClearValue(GetSelectedBlackboardKey());
				}*/
				OwnerComp.GetBlackboardComponent()->
					SetValueAsObject(GetSelectedBlackboardKey(), Player);
			}

			else
			{
				UE_LOG(LogTemp, Error, TEXT("Find Player OwnerComp.GetAIOwner Error"));
				return;
			}
		}
	}
}
