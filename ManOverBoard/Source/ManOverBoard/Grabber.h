// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANOVERBOARD_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab();
	void Release();
	void FindPhysicsComponent();
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns current end of reach line
	FVector GetReachLineStart();

	//Returns current end of reach line
	FVector GetReachLineEnd();
private:
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;
};
