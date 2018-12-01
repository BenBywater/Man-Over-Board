// Fill out your copyright notice in the Description page of Project Settings.

#include "ManOverBoard.h"
#include <math.h>
#include "BuoyancyComponent.h"
#include "BoatPlayer.h"


// Sets default values
ABoatPlayer::ABoatPlayer():
	BoatMeshComponent(nullptr),
	WindowMeshComponent(nullptr),
	Camera1(nullptr),
	CameraArm1(nullptr),
	buoyancy(nullptr),
	currentX(0.f),
	currentY(0.f),
	boatSpeedMax(850),
	boatSpeed(0)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//this->AutoPossessPlayer = EAutoReceiveInput::Player0;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Boat/Boat"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> windowMesh(TEXT("/Game/StarterContent/Props/SM_GlassWindow"));

	// Create the mesh component
	BoatMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PirateBoat"));
	RootComponent = BoatMeshComponent;
	BoatMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	BoatMeshComponent->SetStaticMesh(BoatMesh.Object);
	BoatMeshComponent->SetRelativeScale3D(FVector(25.f, 30.f, 25.f));
	BoatMeshComponent->SetSimulatePhysics(false);
	BoatMeshComponent->SetEnableGravity(true);
	BoatMeshComponent->SetLinearDamping(0.01);

	WindowMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Window"));
	WindowMeshComponent->SetStaticMesh(windowMesh.Object);
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	WindowMeshComponent->AttachToComponent(BoatMeshComponent, rules, TEXT("Socket"));

	UE_LOG(LogTemp, Warning, TEXT("WindowMeshComponent->AttachToComponent"));
	// Create a camera arm for player perspective
	CameraArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm1"));
	UE_LOG(LogTemp, Warning, TEXT("CameraArm1 = CreateDefaultSubobject"));
	CameraArm1->SetupAttachment(RootComponent);
	CameraArm1->bAbsoluteRotation = false; // Don't want arm to rotate when ship does
	CameraArm1->TargetArmLength = 4000.f;
	CameraArm1->RelativeRotation = FRotator(-30.f, 90.f, 0.f);
	CameraArm1->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	//Create a camera
	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	Camera1->SetupAttachment(CameraArm1, USpringArmComponent::SocketName);
	Camera1->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
	AddOwnedComponent(buoyancy);

}

// Called when the game starts or when spawned
void ABoatPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoatPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BobbingBoat(DeltaTime);
	MoveBoat(DeltaTime);
	//RotateBoatRoll(DeltaTime);

}

// Called to bind functionality to input
void ABoatPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ABoatPlayer::SetXValue(float x)
{
	//UE_LOG(LogTemp, Warning, TEXT("X Value: %s"), *FString::Printf(TEXT("%f"), x))
	currentX = x;
}

void ABoatPlayer::SetYValue(float y)
{
	//UE_LOG(LogTemp, Warning, TEXT("Y Value: %s"), *FString::Printf(TEXT("%f"), y))
	currentY = y;
}

void ABoatPlayer::CalculateSpeed(bool moving)
{
	if (moving)
	{
		if (boatSpeed < boatSpeedMax)
		{
			boatSpeed += 1;
		}
	}
	else
	{
		if(boatSpeed > 0)
		{
			boatSpeed -= 1;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("boat speede: %s"), *FString::Printf(TEXT("%f"), boatSpeed));
}



void ABoatPlayer::MoveBoat(float dTime)
{
	FRotator NewRotation = BoatMeshComponent->RelativeRotation;

	if (currentY > 0)
	{
		FVector cameraForward = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector();

		cameraForward.Z = 0.0f;
		cameraForward.Normalize();

		// calculate rotation
		if (currentY != 0)
		{
			if (currentX < 0)
			{
				NewRotation.Yaw += -0.05;
			}
			else if (currentX > 0)
			{
				NewRotation.Yaw += 0.05;
			}
		}


		//FVector MoveDirection = CameraArm1->GetForwardVector() * 1.5;
		FVector MoveDirection = BoatMeshComponent->GetRightVector() * 1.5;
		MoveDirection.Z = 0;
		CalculateSpeed(true);
		// Calculate movement based on direction, speed of player and frame rate dependancy
		const FVector Movement = MoveDirection * boatSpeed * dTime;

		FHitResult Hit(1.f);
		//// move player boat
		BoatMeshComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			BoatMeshComponent->MoveComponent(Deflection, NewRotation, true);
		}
	
	}
	else
	{
			if (NewRotation.Pitch > 0)
			{
				NewRotation.Pitch += -0.05;
			}

			if (NewRotation.Pitch < 0)
			{
				NewRotation.Pitch += 0.05;
			}

		
		CalculateSpeed(false);
		//FVector MoveDirection = CameraArm1->GetForwardVector() * 1.5;
		FVector MoveDirection = BoatMeshComponent->GetRightVector() * 1.5;
		MoveDirection.Z = 0;
		// Calculate movement based on direction, speed of player and frame rate dependancy
		const FVector Movement = MoveDirection * boatSpeed * dTime;

		FHitResult Hit(1.f);
		//// move player boat
		BoatMeshComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			BoatMeshComponent->MoveComponent(Deflection, NewRotation, true);
		}
		SetActorRelativeRotation(NewRotation);
	}
}