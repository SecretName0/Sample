// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CarculatingDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ViolinZombie.h"

UBTService_CarculatingDistance::UBTService_CarculatingDistance()
{
	NodeName = (TEXT("Calculating Distance"));
}

void UBTService_CarculatingDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector PlayerLocation = Player->GetActorLocation();
	FVector MyLocation = OwnerComp.GetOwner()->GetActorLocation();

	float Distance = FVector::Dist(PlayerLocation, MyLocation);
	float MinimumDistance = 800;

	if(Distance >= MinimumDistance)
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsBool(GetSelectedBlackboardKey(), true);
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->
			SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
