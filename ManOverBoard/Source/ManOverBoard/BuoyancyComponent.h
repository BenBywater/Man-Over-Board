// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "BuoyancyComponent.generated.h"

class BoatPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANOVERBOARD_API UBuoyancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuoyancyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RotateBoat(float dTime);
	void BobbingBoat(float dTime);

	bool RotateBoatRight(float dTime);
	bool RotateBoatLeft(float dTime);

private:
	USceneComponent* BoatComponent;

	float waveAmp1;
	float waveAmp2;
	float waveAmp3;

	float waveFreq1;
	float waveFreq2;
	float waveFreq3;

	float bobCycle1;
	float bobCycle2;
	float bobCycle3;

	float lastDegree;
	float pitchSum;
	TArray<float> pitchList; 

	bool turnLeft;
	bool turnRight;
	bool continueRight;
	
};
