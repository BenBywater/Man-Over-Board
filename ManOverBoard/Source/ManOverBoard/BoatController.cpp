// Fill out your copyright notice in the Description page of Project Settings.

#include "ManOverBoard.h"
#include "BoatPlayer.h"
#include "Barrel.h"
#include "BoatController.h"


ABoatController::ABoatController():
	boatPawn(nullptr),
	barrelPawn(nullptr),
	inputX(0.f),
	inputY(0.f)
{
	
}

void ABoatController::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Boat
	FVector boatlocation = FVector(-10730.f, -28540.f, 830.f);
	FRotator rotate = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnInfoboat;
	boatPawn = GetWorld()->SpawnActor<ABoatPlayer>(ABoatPlayer::StaticClass(), boatlocation, rotate, SpawnInfoboat);
	this->Possess(boatPawn);

	/*FVector barrellocation = FVector(200.f, -20500.f, 350.f);
	FRotator rotateBarrel = FRotator(0.f, 0.f, -20.f);
	FActorSpawnParameters SpawnInfobarrel;
	barrelPawn = GetWorld()->SpawnActor<ABarrel>(ABarrel::StaticClass(), barrellocation, rotateBarrel, SpawnInfobarrel);
	barrelPawn->SetActorScale3D(FVector(0.25, 0.25, 0.25));*/
}


void ABoatController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

void ABoatController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("XAxisMovementBoat", this, &ABoatController::PassXValue);
	InputComponent->BindAxis("YAxisMovementBoat", this, &ABoatController::PassYValue);
	
}

void ABoatController::PassXValue(float x)
{
	if (boatPawn != nullptr)
	{
		boatPawn->SetXValue(x);
	}
}

void ABoatController::PassYValue(float y)
{
	if (boatPawn != nullptr)
	{
		boatPawn->SetYValue(y);
	}
}