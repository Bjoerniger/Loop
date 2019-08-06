// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bodyTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("BodyTriggerVolume"));
	if (!ensure(bodyTriggerVolume != nullptr)) return;
	RootComponent = bodyTriggerVolume;
	//bodyTriggerVolume->SetupAttachment(RootComponent);
	bodyTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

	model_static = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_static"));
	model_static->SetupAttachment(RootComponent);
	model_static->bEditableWhenInherited = true;

	model_weapon = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_weapon"));
	model_weapon->SetupAttachment(RootComponent);
	model_weapon->bEditableWhenInherited = true;

	headTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("HeadTriggerVolume"));
	if (!ensure(headTriggerVolume != nullptr)) return;
	headTriggerVolume->SetupAttachment(model_weapon);
	headTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	headTriggerVolume->bEditableWhenInherited = true;

	model_destruct = CreateDefaultSubobject<UDestructibleComponent>(FName("model_destruct"));
	model_destruct->SetupAttachment(RootComponent);
	model_destruct->bEditableWhenInherited = true;

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
			aggroState = EAggroState::EAS_attack;
			Attack();
		}
	}
	else {
		aggroTimerValue = 0.f;
		aggroState = EAggroState::EAS_neutral;
	}

	if (healthValue <= 0) {
		bIsAlive = false;
	}
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!bIsAlive) return;
	UPrimitiveComponent* overlappedComp = SweepResult.GetComponent();
	FString hitComp = overlappedComp->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ">> " + hitComp);
	// if player gets into range (comming from BP)
	if (hitComp == "aggroRange") {
		FString returno = overlappedComp->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "icu!");
		aggroState = EAggroState::EAS_allert;
		bCanSeePlayer = true;
	}
	// if headshot
	if (hitComp == "headTriggerVolume") {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "critical hit!");
		bBeenCrit = true;
		theHitComp = OtherComp;
		theHitMan = OtherActor;
	}
	// if bodyhit by player bullet (comming from cpp)
	if (hitComp == "bodyTriggerVolume") {
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
