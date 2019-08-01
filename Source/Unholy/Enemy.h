// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

UCLASS()
class UNHOLY_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Attack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float aggroTimerValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float aggroTimerMax;

	// overlap component for dmg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* TriggerVolume;

	// overlap component for aggro
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* aggroSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageCast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthReturnValue;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UWidget* ref_healthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashedTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashedTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBeenDashed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGotGrav;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* theHitMan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* theHitComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBeenHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector playerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float shotFrequencyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float shotFrequencyMax;
};
