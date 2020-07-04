// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere);
	float OpenCloseYawDiff = 90.0f;
	UPROPERTY(EditAnywhere);
	ATriggerVolume* OpenPlate = nullptr;
	UPROPERTY(EditAnywhere);
	float DoorCloseDelay = 1.0f;
	UPROPERTY(EditAnywhere);
	float OpenDoorSpeed = 10.0f;
	UPROPERTY(EditAnywhere);
	float CloseDoorSpeed = 4.0f;
	UPROPERTY(EditAnywhere);
	float OpenDoorRange = 1.0f;
	UPROPERTY(EditAnywhere);
	float CloseDoorRange = 1.0f;

	float DoorLastOpened;
	bool IsOpening = false;
	bool IsClosing = false;
	bool DoorIsOpen = false;
	bool DoorIsClosed = true;
	FRotator StartingRotation;
	FRotator CurrentRotation;
	FRotator OpenDoorRot;
	FRotator CloseDoorRot;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
