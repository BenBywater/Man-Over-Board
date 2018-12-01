// Fill out your copyright notice in the Description page of Project Settings.

#include "ManOverBoard.h"
#include "BoatPlayer.h"
#include "BuoyancyComponent.h"


// Sets default values for this component's properties
UBuoyancyComponent::UBuoyancyComponent():
	waveAmp1(0.9),
	waveAmp2(1),
	waveAmp3(0.8),
	waveFreq1(1.2),
	waveFreq2(1),
	waveFreq3(1.5),
	bobCycle1(0.f),
	bobCycle2(0.f),
	bobCycle3(0.f),
	turnLeft(false),
	turnRight(false),
	lastDegree(-10.f),
	BoatComponent(nullptr),
	continueRight(true),
	pitchSum(0.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UBuoyancyComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ByuoyancyComponent begin play"));
	
}


// Called every frame
void UBuoyancyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BobbingBoat(DeltaTime);
	RotateBoat(DeltaTime);
}

void UBuoyancyComponent::BobbingBoat(float dTime)
{
	// convert dTime into x
	float xWave1 = dTime * 360;
	xWave1 /= waveFreq1;
	bobCycle1 += xWave1;
	bobCycle1 = fmod(bobCycle1, 360);
	float degree1 = bobCycle1 * 0.01745329251;
	float bobCycle1Sine = sin(degree1) * waveAmp1;

	float xWave2 = dTime * 360;
	xWave2 /= waveFreq2;
	bobCycle2 += xWave2;
	bobCycle2 = fmod(bobCycle2, 360);
	float degree2 = bobCycle2 * 0.01745329251;
	float bobCycle2Sine = sin(degree2) * waveAmp2;

	float xWave3 = dTime * 360;
	xWave3 /= waveFreq3;
	bobCycle3 += xWave3;
	bobCycle3 = fmod(bobCycle3, 360);
	float degree3 = bobCycle3 * 0.01745329251;
	float bobCycle3Sine = sin(degree3) * waveAmp3;

	//combine all wave vectors into one to create rotation
	FVector loc = GetOwner()->GetRootComponent()->RelativeLocation;
	FVector newLoc = loc;
	float zPlane = bobCycle1Sine + bobCycle2Sine + bobCycle3Sine;

	newLoc.Z += zPlane;
	GetOwner()->GetRootComponent()->SetRelativeLocation(newLoc);
}

bool UBuoyancyComponent::RotateBoatRight(float dTime)
{
	float degree = 0.f;

	if (turnLeft != true)
	{
		float rightPitch = dTime * 360;
		rightPitch /= 2.f; 
		pitchSum += rightPitch;
		pitchSum = fmod(pitchSum, 360);
		degree = pitchSum * 0.01745329251;

		if (lastDegree > degree)
		{
			turnLeft = true;
		}
		else
		{
			FRotator NewRotation = GetOwner()->GetRootComponent()->RelativeRotation;

			if (degree > 0)
			{
				NewRotation.Pitch = degree;
				GetOwner()->SetActorRelativeRotation(NewRotation);
				pitchList.Push(degree);
				lastDegree = degree;
			}
		}

	}
	else
	{
		FRotator NewRotation = GetOwner()->GetRootComponent()->RelativeRotation;
		NewRotation.Pitch = pitchList.Last();
		GetOwner()->SetActorRelativeRotation(NewRotation);
		pitchList.Pop();
		if (pitchList.Num() == 0)
		{
			lastDegree = 10.f;
			turnLeft = false;
			return false;
		}
	}

	return true; // return true is continue to rotate boat right
}

bool UBuoyancyComponent::RotateBoatLeft(float dTime)
{
	float degree = 0.f;

	if (turnRight != true)
	{
		float leftPitch = dTime * 360;
		leftPitch /= 2.f;
		pitchSum += leftPitch;
		pitchSum = fmod(pitchSum, 360);
		degree = pitchSum * 0.01745329251;
		degree = 1 - degree;

		if (lastDegree < degree)
		{
			turnRight = true;
		}
		else
		{
			FRotator NewRotation = GetOwner()->GetRootComponent()->RelativeRotation;

			if (degree < 0)
			{
				NewRotation.Pitch = degree;
				GetOwner()->SetActorRelativeRotation(NewRotation);
				pitchList.Push(degree);
				lastDegree = degree;
			}
		}
	}
	else
	{
		FRotator NewRotation = GetOwner()->GetRootComponent()->RelativeRotation;
		NewRotation.Pitch = pitchList.Last();
		GetOwner()->SetActorRelativeRotation(NewRotation);
		pitchList.Pop();
		if (pitchList.Num() == 0)
		{
			lastDegree = -10.f;
			turnRight = false;
			return true; // return true to change 
		}
	}
	return false; // return true is continue to rotate boat backward
}

void UBuoyancyComponent::RotateBoat(float dTime)
{

	if (continueRight)
	{
		continueRight = RotateBoatRight(dTime);
	}
	else
	{
		continueRight = RotateBoatLeft(dTime);
	}
}