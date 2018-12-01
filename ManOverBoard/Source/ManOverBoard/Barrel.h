// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Barrel.generated.h"

UCLASS()
class MANOVERBOARD_API ABarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrel();
	void BobbingBoat(float dTime);
	void RotateBoatRoll(float dTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UStaticMeshComponent* BarrelComponent;

	float waveAmp1;
	float waveAmp2;
	float waveAmp3;

	float waveFreq1;
	float waveFreq2;
	float waveFreq3;

	float bobCycle1;
	float bobCycle2;
	float bobCycle3;

	float reverseRoll;
	float swivelY;
	bool reversePitch;
	bool reverseLock;
	bool negativeLock;
	float pDegree2;
	float nDegree2;
	TArray<float> pitchList;

	float boatSpeedMax;
	float boatSpeed;
	
	
};
