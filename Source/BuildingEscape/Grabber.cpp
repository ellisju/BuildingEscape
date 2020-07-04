// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//Checking for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}

	// Check for the Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::ToggleGrab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No input component found on %s!"), *GetOwner()->GetName());
	}


	// UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	// Check for the Physics Handle Component.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent attached to grabber component of object %s!"), *GetOwner()->GetName());
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		OUT FVector PlayerViewLocation;
		OUT FRotator PlayerViewRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
		FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::ToggleGrab() {
	if (PhysicsHandle->GrabbedComponent) {
		Release();
	}
	else {
		Grab();
	}
}

void UGrabber::Grab() {
	FHitResult Hit = GetBodyWithinReach();

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.ToString());

	// AActor* Actor = Hit.GetActor();

	if (Hit.GetActor()) {
		//UE_LOG(LogTemp, Warning, TEXT("Hit Object: %s"), *Hit.GetActor()->GetName());

		UPrimitiveComponent* Component = Hit.GetComponent();
		PhysicsHandle->GrabComponentAtLocation(
			Component,
			NAME_None,
			Hit.GetActor()->GetActorLocation()
		);
	}
}

void UGrabber::Release() {
	//UE_LOG(LogTemp, Warning, TEXT("You let go!"));
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetBodyWithinReach() const {
	OUT FVector PlayerViewLocation;
	OUT FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	// UE_LOG(LogTemp, Warning, TEXT("View Loc: %s, View Rot: %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	// DrawDebugLine(GetWorld(), PlayerViewLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, DebugLineThickness);

	OUT FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	bool ObjectInRange = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}