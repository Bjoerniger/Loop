// Fill out your copyright notice in the Description page of Project Settings.

#include "UnholyPlayerController.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"



AUnholyPlayerController::AUnholyPlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	PrimaryActorTick.bCanEverTick = true;
}

void AUnholyPlayerController::Tick(float deltaTime) {

}