// Fill out your copyright notice in the Description page of Project Settings.


#include "NavPointActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANavPointActor::ANavPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMeshComponent* SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereMesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereMeshAsset.Succeeded()) {
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		SphereMesh->SetWorldScale3D(FVector(0.1f));
		//SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

// Called when the game starts or when spawned
void ANavPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavPointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

