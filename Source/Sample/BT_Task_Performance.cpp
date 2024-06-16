// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_Performance.h"
#include "ViolinZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBT_Task_Performance::UBT_Task_Performance()
{
	NodeName = (TEXT("Performance Pattern"));
}

EBTNodeResult::Type UBT_Task_Performance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetOwnerInfo(OwnerComp);

	OwnerZombie->PerformanceEffect();

	return EBTNodeResult::Succeeded;
}

void UBT_Task_Performance::GetOwnerInfo(UBehaviorTreeComponent& OwnerComp)
{
	VZController = Cast<AViolinZombieController>
		(OwnerComp.GetAIOwner());

	if (VZController)
	{
		VZ_BB = VZController->GetBlackboardComponent();

		if (!VZ_BB)
		{
			UE_LOG(LogTemp, Warning, TEXT("NO BBC"));
			return;
		}

		else
		{
			if (VZ_BB->GetValueAsObject("Player"))
			{
				OwnerZombie = Cast<AViolinZombie>(VZController->GetPawn());
			}

			if (!OwnerZombie)
			{
				UE_LOG(LogTemp, Warning, TEXT("NO OwnerZombie"));
				return;
			}
		}
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO VZCont"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Roar Data Load Success!"));
}

