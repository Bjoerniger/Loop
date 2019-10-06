// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "NPC_Enemy.generated.h"

UCLASS()
class UNHOLY_API ANPC_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// overlap component for dmg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* bodyTriggerVolume;

	// overlap component for dmg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* headTriggerVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* model_weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDestructibleComponent* model_destruct;

};
