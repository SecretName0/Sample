// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "Tank_Controller.h"
#include "TankZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

// ÅÊÅ© Àü¿ë

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = (TEXT("Tank Pattern"));
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ATank_Controller* TankController =
        Cast<ATank_Controller>(OwnerComp.GetAIOwner());

    if (TankController)
    {
        ATankZombie* Tank =
            Cast<ATankZombie>(TankController->GetPawn());

        if (Tank)
        {
            if(TankController->BlackBoardComponent->GetValueAsBool("CanThrow"))
            {
                Tank->TankThrowAttack();

                TankController->BlackBoardComponent->
                    SetValueAsBool("CanThrow", false);
            }

            else
            {
                Tank->AIMeleeAttack();
            }

            return EBTNodeResult::Succeeded;
        }

        else
        {
            UE_LOG(LogTemp, Error, TEXT("Melee Script Chara Error"));
        }
    }

    else
    {
        UE_LOG(LogTemp, Error, TEXT("Melee Script Controller Error"));
    }

    return EBTNodeResult::Failed;
}
