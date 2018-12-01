// Fill out your copyright notice in the Description page of Project Settings.

#include "ManOverBoard.h"
#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();

}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached InputComponent 
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Line-trace (AKA Ray Cast)
	FHitResult HitReslt;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitReslt,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitReslt;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

void UGrabber::Grab()
{
	///LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	// gets mesh
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// if we hit something then attach physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
}

void UGrabber::Release()
{
	/*if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		UPrimitiveComponent* item = PhysicsHandle->GetGrabbedComponent();
		PhysicsHandle->ReleaseComponent();
		item->AddImpulse(FVector(1000.f, 0.f, 0.f));
	}*/
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}