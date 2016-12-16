// Copyright Luong Chuong 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

public:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
private:
	UPROPERTY( EditAnywhere )
	float OpenAngle = 90.0f;

	UPROPERTY( EditAnywhere )
	ATriggerVolume* PressurePlate;

	UPROPERTY( EditAnywhere )
	float DoorCloseDelay = 1.0f;

private:
	AActor* Owner; // Owner of the door
	AActor* ActorThatOpens; // Pawn that inherit AActor

	float LastDoorOpenTime;

private:
	virtual void OpenDoor();
	virtual void CloseDoor();
};
