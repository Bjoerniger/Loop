// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AIController.h"
#include "Enemy.h"

AEnemy_AIController::AEnemy_AIController() {
	//Initialize the behavior tree and blackboard components
	/*BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));*/
}

void AEnemy_AIController::BeginPlay() {

	Super::BeginPlay();

	//Get the possessed Pawn. If it's the AI Character we created
	//initialize it's blackboard and start it's corresponding behavior tree
	/*AEnemy* enemy = Cast<AEnemy>(GetOwner());
	if (enemy)
	{
		if (enemy->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(enemy->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*enemy->BehaviorTree);
		}
	}*/
}

void AEnemy_AIController::SetSeenTarget(APawn* Pawn) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "well hello there, " + Pawn->GetName());
	target = Pawn;
	//Registers the Pawn that the AI has seen in the blackboard
	//if (BlackboardComp)
	//{
	//	BlackboardComp->SetValueAsObject(BlackboardKey, Pawn);
	//}
}