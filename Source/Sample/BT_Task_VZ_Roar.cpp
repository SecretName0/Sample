// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_VZ_Roar.h"

UBT_Task_VZ_Roar::UBT_Task_VZ_Roar()
{
	NodeName = (TEXT("Roar Pattern"));
}

EBTNodeResult::Type UBT_Task_VZ_Roar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetOwnerInfo(OwnerComp);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this,
		&UBT_Task_VZ_Roar::Roar, 1, false);
	
	FTimerHandle CoolTime;
	GetWorld()->GetTimerManager().SetTimer(CoolTime, this,
		&UBT_Task_VZ_Roar::CoolDown, 7, false);

	return EBTNodeResult::Succeeded;
}

void UBT_Task_VZ_Roar::GetOwnerInfo(UBehaviorTreeComponent& OwnerComp)
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

void UBT_Task_VZ_Roar::Roar()
{
	OwnerZombie->RoarEffect();
}

void UBT_Task_VZ_Roar::CoolDown()
{
	VZController->BlackBoardComponent->SetValueAsBool("CanRoar", true);
}

