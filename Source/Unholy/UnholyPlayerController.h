// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnholyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNHOLY_API AUnholyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	AUnholyPlayerController();
	
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

};

