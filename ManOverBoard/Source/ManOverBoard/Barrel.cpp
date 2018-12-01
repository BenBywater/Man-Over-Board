// Fill out your copyright notice in the Description page of Project Settings.

#include "ManOverBoard.h"
#include "Barrel.h"


// Sets default values
ABarrel::ABarrel():
	waveAmp1(0.9),
	waveAmp2(1),
	waveAmp3(0.8),
	waveFreq1(1.5),
	waveFreq2(1),
	waveFreq3(2),
	bobCycle1(0.f),
	bobCycle2(0.f),
	bobCycle3(0.f),
	boatSpeedMax(850),
	swivelY(-100.f),
	reverseRoll(6.f),
	reversePitch(true),
	reverseLock(true),
	negativeLock(false),
	nDegree2(10.f),
	pDegree2(-10.f),
	boatSpeed(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("/Game/Barrel/Barrell_Oak_FBX"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("Material'/Game/Textures/hand_Mat"));

	BarrelComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloatingBarrel"));
	RootComponent = BarrelComponent;
	BarrelComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	BarrelComponent->SetStaticMesh(barrelMesh.Object);
	BarrelComponent->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	BarrelComponent->SetSimulatePhysics(false);
	BarrelComponent->SetEnableGravity(true);
	BarrelComponent->SetLinearDamping(0.01);

}

// Called when the game starts or when spawned
void ABarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BobbingBoat(DeltaTime);
	RotateBoatRoll(DeltaTime);

}

void ABarrel::BobbingBoat(float dTime)
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
	FVector loc = BarrelComponent->RelativeLocation;
	FVector newLoc = loc;
	float zPlane = bobCycle1Sine + bobCycle2Sine + bobCycle3Sine;

	newLoc.Z += zPlane;
	newLoc.Y += 4;
	BarrelComponent->SetRelativeLocation(newLoc);
}

void ABarrel::RotateBoatRoll(float dTime)
{
	float degree1 = 100;

	if (negativeLock != true)
	{
		if (reverseLock != false)
		{
			float pitch = dTime * 360;
			pitch /= 2.f;
			swivelY += pitch;
			swivelY = fmod(swivelY, 360);
			degree1 = swivelY * 0.01745329251;
			degree1 *= 4;

			if (pDegree2 > degree1)
			{
				reverseLock = false;
			}
			else
			{
				FRotator NewRotation = BarrelComponent->RelativeRotation;

				if (degree1 > 0)
				{
					NewRotation.Pitch = degree1;
					SetActorRelativeRotation(NewRotation);
					pitchList.Push(degree1);
					pDegree2 = degree1;
				}
				else
				{
					NewRotation.Pitch = 0;
					SetActorRelativeRotation(NewRotation);
					pitchList.Push(0);
					pDegree2 = 0;
				}
			}

		}
		else
		{
			FRotator NewRotation = BarrelComponent->RelativeRotation;
			NewRotation.Pitch = pitchList.Last();
			SetActorRelativeRotation(NewRotation);
			pitchList.Pop();
			if (pitchList.Num() == 0)
			{
				reverseLock = true;
				negativeLock = true;
				pDegree2 = -10.f;
			}
		}
	}
	else
	{
		if (reverseLock != false)
		{
			float pitch = dTime * 360;
			pitch /= 2.f;
			swivelY += pitch;
			swivelY = fmod(swivelY, 360);
			degree1 = swivelY * 0.01745329251;
			degree1 = (1 - degree1)*4;

			if (nDegree2 < degree1)
			{
				reverseLock = false;
			}
			else
			{
				FRotator NewRotation = BarrelComponent->RelativeRotation;

				if (degree1 < 0)
				{
					NewRotation.Pitch = degree1;
					SetActorRelativeRotation(NewRotation);
					pitchList.Push(degree1);
					nDegree2 = degree1;
				}
			}
		}
		else
		{
			FRotator NewRotation = BarrelComponent->RelativeRotation;
			NewRotation.Pitch = pitchList.Last();
			SetActorRelativeRotation(NewRotation);
			pitchList.Pop();
			if (pitchList.Num() == 0)
			{
				reverseLock = true;
				negativeLock = false;
				nDegree2 = 10.f;
			}
		}
	}
	//	UE_LOG(LogTemp, Warning, TEXT("reverse Value: %s"), *FString::Printf(TEXT("%f"), degree2));
}