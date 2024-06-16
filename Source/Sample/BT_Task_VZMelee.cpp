// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_VZMelee.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "ViolinZombie.h"
#include "ViolinZombieController.h"

UBT_Task_VZMelee::UBT_Task_VZMelee()
{
	NodeName = "VZ Melee";
}

EBTNodeResult::Type UBT_Task_VZMelee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AViolinZombieController* VZC = Cast<AViolinZombieController>
        (OwnerComp.GetAIOwner());

    AViolinZombie* VZ = Cast<AViolinZombie>(VZC->GetPawn());
    VZ->AIMeleeAttack();
    UE_LOG(LogTemp, Display, TEXT("VZ Melee"));

    return EBTNodeResult::Succeeded;
}


