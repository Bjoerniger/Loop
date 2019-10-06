// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Enemy.h"

// Sets default values
ANPC_Enemy::ANPC_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bodyTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("BodyTriggerVolume"));
	if (!ensure(bodyTriggerVolume != nullptr)) return;
	bodyTriggerVolume->SetupAttachment(RootComponent);
	//bodyTriggerVolume->SetupAttachment(RootComponent);
	bodyTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ANPC_Enemy::OnOverlapBegin);
	bodyTriggerVolume->bEditableWhenInherited = true;

	//model_static = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_static"));
	//model_static->SetupAttachment(RootComponent);
	//model_static->bEditableWhenInherited = true;

	model_weapon = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_weapon"));
	model_weapon->SetupAttachment(GetMesh());
	model_weapon->bEditableWhenInherited = true;

	headTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("HeadTriggerVolume"));
	if (!ensure(headTriggerVolume != nullptr)) return;
	headTriggerVolume->SetupAttachment(model_weapon);
	headTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ANPC_Enemy::OnOverlapBegin);
	headTriggerVolume->bEditableWhenInherited = true;
	//headTriggerVolume->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

	model_destruct = CreateDefaultSubobject<UDestructibleComponent>(FName("model_destruct"));
	model_destruct->SetupAttachment(RootComponent);
	model_destruct->bEditableWhenInherited = true;

}

// Called when the game starts or when spawned
void ANPC_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Enemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

}

void ANPC_Enemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}