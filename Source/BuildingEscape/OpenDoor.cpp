// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	float MyFloat = 90.0f;

	StartingRotation = GetOwner()->GetActorRotation();
	CurrentRotation = StartingRotation;
	// UE_LOG(LogTemp, Warning, TEXT("Current rotation: %s"), *StartingRotation.ToString());
	CloseDoorRot = StartingRotation;
	OpenDoorRot = FRotator(StartingRotation.Pitch, StartingRotation.Yaw + OpenCloseYawDiff, StartingRotation.Roll);

	ActorOpen = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!OpenPlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has an OpenDoor Component but OpenPlate is NULL!"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CTime = GetWorld()->GetTimeSeconds();
	
	if (OpenPlate && OpenPlate->IsOverlappingActor(ActorOpen)) {
		if (!DoorIsOpen) {
			DoorIsClosed = false;
			IsOpening = true;
			IsClosing = false;
		}
		else {
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
	}
	else if (OpenPlate && !DoorIsClosed && !OpenPlate->IsOverlappingActor(ActorOpen) && DoorLastOpened && GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {
		DoorIsOpen = false;
		IsOpening = false;
		IsClosing = true;
	}
	if (IsOpening) {
		OpenDoor(DeltaTime);
	} else if (IsClosing) {
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	// CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, OpenDoorRot.Yaw, 0.1f);
	// Unlike the similar line above, this one opens the door the same speed regardless of framerate. (1.f below could still be .1f above if that speed works).
	// CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, OpenDoorRot.Yaw, DeltaTime*1.f);
	// CurrentRotation.Yaw = FMath::FInterpConstantTo(CurrentRotation.Yaw, OpenDoorRot.Yaw, DeltaTime, 150);

	// CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, OpenDoorRot.Yaw, DeltaTime, 1);
	CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, OpenDoorRot.Yaw, DeltaTime * OpenDoorSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("Current rotation: %f"), CurrentRotation.Yaw);
	GetOwner()->SetActorRotation(CurrentRotation);
	DoorLastOpened = GetWorld()->GetTimeSeconds();
	if ((OpenDoorRot.Yaw - OpenDoorRange) < CurrentRotation.Yaw && CurrentRotation.Yaw < (OpenDoorRot.Yaw + OpenDoorRange)) {
		DoorIsOpen = true;
		IsOpening = false;
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("Stopped Opening"));
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, CloseDoorRot.Yaw, DeltaTime, CloseDoorSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("Current rotation: %f"), CurrentRotation.Yaw);
	GetOwner()->SetActorRotation(CurrentRotation);
	if ((CloseDoorRot.Yaw - CloseDoorRange) < CurrentRotation.Yaw && CurrentRotation.Yaw < (CloseDoorRot.Yaw + CloseDoorRange)) {
		DoorIsClosed = true;
		IsClosing = false;
		// UE_LOG(LogTemp, Warning, TEXT("Stopped Closing"));
	}
}

