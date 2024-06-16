// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ATank_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATank_Controller::BeginPlay()
{
	Super::BeginPlay();

	//CanThrow = true; // �� 1ȸ�� ��ô ������
	BlackBoardComponent->SetValueAsBool("CanThrow", true);
}