// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Engine/World.h"
#include "NavPointActor.h"
#include "GameFramework/Actor.h"
#include "NavCube.generated.h"

UCLASS()
class UNHOLY_API ANavCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* navCubeRange;

	// actual size of the navcube:
	// x
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	float navCube_width;

	// y
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	float navCube_depth;

	// z
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cube")
	float navCube_height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ANavPointActor> navPointActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float navMeshDensityX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float navMeshDensityY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float navMeshDensityZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool placeNavPoints;
};
