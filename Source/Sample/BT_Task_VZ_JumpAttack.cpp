// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_VZ_JumpAttack.h"
#include "Kismet/GameplayStatics.h"

UBT_Task_VZ_JumpAttack::UBT_Task_VZ_JumpAttack()
{
	NodeName = "VZ Jump Attack";
}

EBTNodeResult::Type UBT_Task_VZ_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetOwnerInfo(OwnerComp);
	AMyCharacter* Player = Cast<AMyCharacter>(VZController->BlackBoardComponent
		->GetValueAsObject("Player"));

	float AdjustPower = 400;
	Jumping(OwnerZombie, Player, AdjustPower);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this,
		&UBT_Task_VZ_JumpAttack::CoolDown, 10, false);

	return EBTNodeResult::Succeeded;
}

void UBT_Task_VZ_JumpAttack::OverJumping(AViolinZombie* Owner, AMyCharacter* TargetPlayer)
{
	// 플레이어의 배후까지 날아감
	float LaunchAngle = 45;
	FVector VZLocation = OwnerZombie->GetActorLocation();
	FVector TargetLocation = TargetPlayer->GetActorLocation();

	FVector JumpVelocity = CalculatingJumpVelocity(VZLocation,
		TargetLocation, LaunchAngle);
	
	OwnerZombie->LaunchCharacter(JumpVelocity, true, true);
}

void UBT_Task_VZ_JumpAttack::Jumping(AViolinZombie* Owner, AMyCharacter* TargetPlayer, float AfterJumpDistance)
{
	float LaunchAngle = 45;
	FVector VZLocation = OwnerZombie->GetActorLocation();
	FVector TargetLocation = TargetPlayer->GetActorLocation();

	FVector AdjustedTargetLocation = CalculatingAdjustJumpDistance
	(TargetLocation, VZLocation, AfterJumpDistance);

	FVector JumpVelocity = CalculatingJumpVelocity(VZLocation,
		AdjustedTargetLocation, LaunchAngle);

	OwnerZombie->LaunchCharacter(JumpVelocity, true, true);
}

FVector UBT_Task_VZ_JumpAttack::CalculatingJumpVelocity(FVector VZLoca, FVector TargetLoca, float LaunchAngle)
{
	float Gravity = FMath::Abs(GetWorld()->GetGravityZ());
	FVector Dir = TargetLoca - VZLoca;
	float Dist = FVector::Dist(VZLoca, TargetLoca);

	float SinAngle, CosAngle;
	FMath::SinCos(&SinAngle, &CosAngle, FMath::DegreesToRadians(LaunchAngle));

	float V0 = FMath::Sqrt(Gravity * Dist / (2 * SinAngle * CosAngle));

	FVector Velocity = V0 * Dir.GetSafeNormal2D();
	Velocity.Z = V0 * SinAngle;
	
	return Velocity;
}

FVector UBT_Task_VZ_JumpAttack::CalculatingAdjustJumpDistance(FVector PlayerLocation, FVector OwnerLocation, float DesiredDistance)
{
	FVector Dir = (PlayerLocation - OwnerLocation).GetSafeNormal();
	FVector AdjustJumpDistance = PlayerLocation - (Dir * DesiredDistance);
	return AdjustJumpDistance;
}

void UBT_Task_VZ_JumpAttack::GetOwnerInfo(UBehaviorTreeComponent& OwnerComp)
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

void UBT_Task_VZ_JumpAttack::CoolDown()
{
	VZController->BlackBoardComponent->SetValueAsBool("CanJumpAttack", true);
}
