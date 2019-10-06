// Fill out your copyright notice in the Description page of Project Settings.


#include "NavCube.h"

// Sets default values
ANavCube::ANavCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	navMeshDensityX = 10.f;
	navMeshDensityY = 10.f;
	navMeshDensityZ = 10.f;

	placeNavPoints = false;
}

// Called when the game starts or when spawned
void ANavCube::BeginPlay()
{
	Super::BeginPlay();
	
	if (navCubeRange->IsValidLowLevel()) {
		navCube_width = navCubeRange->GetComponentScale().Y;
		navCube_height = navCubeRange->GetComponentScale().Z;
		navCube_depth = navCubeRange->GetComponentScale().X;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(navCube_width) + ", " + FString::SanitizeFloat(navCube_height) + ", " + FString::SanitizeFloat(navCube_depth));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "no navCube ref!");
	}
}

// Called every frame
void ANavCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (placeNavPoints) {
		FVector min;
		FVector max;
		navCubeRange->GetLocalBounds(min, max);
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, "min/max: " + min.ToString() + ", " + max.ToString());

		FVector minScaled = min * navCubeRange->GetComponentScale();
		FVector maxScaled = max * navCubeRange->GetComponentScale();
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, "min/max (scaled): " + minScaled.ToString() + ", " + maxScaled.ToString());

		float width = -minScaled.Y + maxScaled.Y;
		float height = -minScaled.Z + maxScaled.Z;
		//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Green, "width/height: " + FString::SanitizeFloat(width) + ", " + FString::SanitizeFloat(height));

		float ratio = width / height;

		float magicNumberY = (width / navMeshDensityY);
		float magicNumberZ = (height / navMeshDensityZ);

		float locX = navCubeRange->GetComponentLocation().X;
		float locY = navCubeRange->GetComponentLocation().Y + minScaled.Y;
		float locZ = navCubeRange->GetComponentLocation().Z + minScaled.Z;

		// width
		for (int y = 0; y < navMeshDensityY; y++) {
			// height
			for (int z = 0; z < navMeshDensityZ; z++) {
				FVector Location(locX, locY + (y * magicNumberY), locZ + (z * magicNumberZ));
				FRotator Rotation(0.0f, 0.0f, 0.0f);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
				ANavPointActor* NavActor = GetWorld()->SpawnActor<ANavPointActor>(Location, Rotation, SpawnParams);
				if (NavActor) {
					NavActor->SetActorEnableCollision(false);
				}
			}
		}
	}
	placeNavPoints = false;
}


