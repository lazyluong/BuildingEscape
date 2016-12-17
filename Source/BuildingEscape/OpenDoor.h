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
	float OpenAngle = -90.0f;

	UPROPERTY( EditAnywhere )
	float DoorCloseDelay = 1.0f;

	UPROPERTY( EditAnywhere )
	ATriggerVolume* PressurePlate;

private:
	AActor* Owner; // Owner of the Door
	AActor* ActorThatOpens; // Pawn inherits AActor

	float LastDoorOpentime;

private:
	virtual void OpenDoor();
	virtual void CloseDoor();
};
