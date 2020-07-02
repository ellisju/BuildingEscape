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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OUT FVector PlayerViewLocation;
	OUT FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	// UE_LOG(LogTemp, Warning, TEXT("View Loc: %s, View Rot: %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, DebugLineThickness);
}

