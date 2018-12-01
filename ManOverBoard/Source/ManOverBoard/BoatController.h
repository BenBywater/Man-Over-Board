// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BoatController.generated.h"

class ABoatPlayer;
class ABarrel;
/**
 * 
 */
UCLASS()
class MANOVERBOARD_API ABoatController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABoatController();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void SetupInputComponent() override;
	void PassXValue(float x);
	void PassYValue(float y);

private:
	ABoatPlayer* boatPawn;
	ABarrel* barrelPawn;
	float inputX;
	float inputY;
	
};
