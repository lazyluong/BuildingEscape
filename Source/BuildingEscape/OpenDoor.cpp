// Copyright Luong Chuong 2016

#include "BuildingEscape.h"
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

	// Find the owning Actor
	Owner = GetOwner();

	// Find the player controller
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}



// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger every frame
	// If the ActorThatOpens is in the volume
	if( PressurePlate->IsOverlappingActor( ActorThatOpens ) )
	{
		// Open the door
		OpenDoor();
		LastDoorOpentime = GetWorld()->GetTimeSeconds();
	}

	auto Delta = GetWorld()->GetTimeSeconds() - LastDoorOpentime;
	if( Delta > DoorCloseDelay )
	{
		// Close the door
		CloseDoor();
	}
}



void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	Owner->SetActorRotation( FRotator( 0.0f , OpenAngle , 0.0f ) );
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	Owner->SetActorRotation( FRotator( 0.0f , 0.0f , 0.0f ) );
}