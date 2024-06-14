// Fill out your copyright notice in the Description page of Project Settings.


#include "CppBaseActor.h"

// Sets default values
ACppBaseActor::ACppBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void ACppBaseActor::SinMovement(float DeltaTime)
{
	float RunningTime = GetGameTimeSinceCreation();
	float Time = FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime);
	InitialLocation.Z += Amplitude * Frequency * Time;
	// z  = Amplitude × sin(Frequency × Time) + z0
	SetActorLocation(InitialLocation);
	
}

// Called when the game starts or when spawned
void ACppBaseActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
}

// Called every frame
void ACppBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

