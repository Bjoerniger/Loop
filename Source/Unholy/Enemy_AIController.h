// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class UNHOLY_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*Behavior Tree component reference*/
	UBehaviorTreeComponent* BehaviorComp;

	/*Blackboard comp ref*/
	UBlackboardComponent* BlackboardComp;

public:

	/*Constructor*/
	AEnemy_AIController();

	/*Blackboard key*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";

	/*Sets the sensed target in the blackboard*/
	void SetSeenTarget(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	APawn* target;

};
