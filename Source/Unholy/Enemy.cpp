// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

	// set in BP till you know how to collision
	//aggroSphere = CreateDefaultSubobject<USphereComponent>(FName("aggroSphere"));
	//if (!ensure(aggroSphere != nullptr)) return;
	//aggroSphere->AttachTo(TriggerVolume);
	//aggroSphere->SetSphereRadius(400.0f);
	//aggroSphere->SetGenerateOverlapEvents(true);
	//aggroSphere->SetCollisionProfileName(TEXT("Pawn"));
	//aggroSphere->bEditableWhenInherited = true;
	//aggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	healthMax = 9.f;
	healthValue = healthMax;
	dashedTimerMax = .4f;
	dashedTimerValue = 0.f;
	bBeenDashed = false;
	bIsAlive = true;
	bGotGrav = true;

	aggroTimerValue = 0.f;			// time since enemy can see player
	aggroTimerMax = 1.f;			// time needed to react

	shotFrequencyValue = 0.f;
	shotFrequencyMax = 1.5f;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	healthReturnValue = healthValue / healthMax;

	if (bCanSeePlayer) {
		if (aggroTimerValue < aggroTimerMax) {
			aggroTimerValue += DeltaTime;
		}
		else {
			Attack();
		}
	}
	else {
		aggroTimerValue = 0.f;
	}

	if (healthValue <= 0) {
		bIsAlive = false;
	}
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!bIsAlive) return;
	UPrimitiveComponent* overlappedComp = SweepResult.GetComponent();

	// if player gets into range (comming from BP)
	if (overlappedComp->GetFName() == "aggroRange") {
		FString returno = overlappedComp->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "icu!");

		bCanSeePlayer = true;
	}
	else if (overlappedComp->GetFName() == "aggroSphere") {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ach jetzt doch?!");
	}
	// if hit by player bullet (comming from cpp)
	else if (overlappedComp == TriggerVolume) {
		bBeenHit = true;
		theHitComp = OtherComp;
		theHitMan = OtherActor;
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (!bIsAlive) return;
	if (OtherActor->ActorHasTag("Player")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "nocu!");
		//aggroTimerValue = 0.f;
		bCanSeePlayer = false;
		bShoot = false;
	}
}

void AEnemy::Attack() {
	if (!bIsAlive) return;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "aaaaah kacke!");

	if (shotFrequencyValue >= shotFrequencyMax) {
		bShoot = true;
	}
	else {
		shotFrequencyValue += GetWorld()->GetDeltaSeconds();
	}
	
}
