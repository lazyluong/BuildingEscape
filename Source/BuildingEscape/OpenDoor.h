// Copyright Luong Chuong 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FDoorEvent );

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

public:
	UPROPERTY( BlueprintAssignable )
	FDoorEvent OnOpen;

	UPROPERTY( BlueprintAssignable )
	FDoorEvent OnClose;
	
private:
	UPROPERTY( EditAnywhere )
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY( EditAnywhere )
	float TriggerMass = 10.0f;

private:
	AActor* Owner = nullptr; // Owner of the Door

private:
	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
