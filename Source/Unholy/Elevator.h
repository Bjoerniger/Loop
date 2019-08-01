// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMovementAxis : uint8
{
	EMA_Z 	UMETA(DisplayName = "up/down"),
	EMA_Y 	UMETA(DisplayName = "left/right"),
	EMA_X	UMETA(DisplayName = "away/to_Cam")
};


UCLASS()
class UNHOLY_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EMovementAxis movementAxis;

	// to or from max height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int movementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementSpeed;

	// range to move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementHeight;

	// time to wait @start&end
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pauseTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pauseTimerActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated;

	// start location to compare movement height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector startLocation;

	// start location to compare movement height
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector actorLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* startBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector sBlockLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* endBlock;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector eBlockLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector elevatorLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void MoveOnAxis(int axis, float DeltaTime);
};

