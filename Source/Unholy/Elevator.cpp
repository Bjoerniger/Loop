// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"


// Sets default values
AElevator::AElevator() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	movementDirection = 0;

	movementHeight = 0.f;
	movementSpeed = 2.0f;
	bIsActivated = false;

	pauseTimer = 2.f;
	pauseTimerActive = 0.f;

	movementAxis = EMovementAxis::EMA_Z;
}

// Called when the game starts or when spawned
void AElevator::BeginPlay() {
	Super::BeginPlay();

	startLocation = GetActorLocation();

	TArray<UStaticMeshComponent*> comps;
	GetComponents(comps);
	comps[1]->GetComponentLocation();
	int sizeArray = (int)comps.Num();

	// get location of start- and endblock
	for (int i = 0; i < sizeArray; i++) {
		if (comps[i]->GetFName().ToString() == "StartBlockMesh") {
			sBlockLocation = comps[i]->GetComponentLocation();	
		}
		else if (comps[i]->GetFName().ToString() == "EndBlockMesh") {
			eBlockLocation = comps[i]->GetComponentLocation();
		}
	}
}

// Called every frame
void AElevator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//actorLocation = GetActorLocation();
	if (bIsActivated) {
		// get location of elevator block
		TArray<UStaticMeshComponent*> compsEl;
		GetComponents(compsEl);
		int sizeArray = (int)compsEl.Num();
		for (int i = 0; i < sizeArray; i++) {
			if (compsEl[i]->GetFName().ToString() == "ElevatorMesh") {
				elevatorLocation = compsEl[i]->GetComponentLocation();
			}
		}
		
		MoveOnAxis((int)movementAxis, DeltaTime);
	}
		
}


void AElevator::MoveOnAxis(int axis, float DeltaTime) {
	
	if (movementAxis == EMovementAxis::EMA_Z) {
		// z: up(0)/down(1)
				
		if (movementDirection == 0) {
			if (elevatorLocation.Z < eBlockLocation.Z) {
				AddActorWorldOffset(FVector(0.f, 0.f, movementSpeed));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					movementDirection = 1;
					pauseTimerActive = 0;
				}
			}
			
		}
		else if (movementDirection == 1) {
			if (elevatorLocation.Z > sBlockLocation.Z) {
				AddActorWorldOffset(FVector(0.f, 0.f, -movementSpeed));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					pauseTimerActive = 0;
					movementDirection = 0;
				}
			}
		}	
	}

	else if (movementAxis == EMovementAxis::EMA_Y) {
		// y: right(0)/left(1)

		if (movementDirection == 0) {
			if (elevatorLocation.Y > eBlockLocation.Y) {
				AddActorWorldOffset(FVector(0.f, -movementSpeed, 0.f ));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					pauseTimerActive = 0;
					movementDirection = 1;
				}
			}

		}
		else if (movementDirection == 1) {
			if (elevatorLocation.Y < sBlockLocation.Y) {
				AddActorWorldOffset(FVector(0.f, movementSpeed, 0.f));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					pauseTimerActive = 0;
					movementDirection = 0;
				}
			}
		}
	}

	else if (movementAxis == EMovementAxis::EMA_X) {
		// x: forward(0)/backward(1)

		if (movementDirection == 0) {
			if (elevatorLocation.X < eBlockLocation.X) {
				AddActorWorldOffset(FVector(movementSpeed, 0.f, 0.f));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					pauseTimerActive = 0;
					movementDirection = 1;
				}
			}

		}
		else if (movementDirection == 1) {
			if (elevatorLocation.X > sBlockLocation.X) {
				AddActorWorldOffset(FVector(-movementSpeed, 0.f, 0.f));
			}
			else {
				if (pauseTimerActive < pauseTimer) {
					pauseTimerActive += DeltaTime;
				}
				else {
					pauseTimerActive = 0;
					movementDirection = 0;
				}
			}
		}
	}

}

