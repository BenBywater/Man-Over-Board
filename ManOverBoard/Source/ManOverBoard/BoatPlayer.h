// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BoatPlayer.generated.h"

class UBuoyancyComponent;

UCLASS()
class MANOVERBOARD_API ABoatPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoatPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetXValue(float x);

	void SetYValue(float y);

	void MoveBoat(float dTime);
	void CalculateSpeed(bool moving);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* BoatMeshComponent;
	UStaticMeshComponent* WindowMeshComponent;

	UCameraComponent* Camera1;
	USpringArmComponent* CameraArm1;

	UBuoyancyComponent* buoyancy;

	float currentX;
	float currentY;

	float boatSpeedMax;
	float boatSpeed;
	
};
