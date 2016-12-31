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

	// Find the acting owner
	Owner = GetOwner();

	if( PressurePlate == nullptr )
	{
		UE_LOG( LogTemp , Error , 
			TEXT( "%s is missing a Pressure Plate." ) ,
			*Owner->GetName() );
	}
}



// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger every frame
	if( GetTotalMassOfActorsOnPlate() >= TriggerMass )
	{
		// Open the door
		OnOpen.Broadcast();
	}
	else
	{
		// Close the door
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	// Find all the overlapping actors
	if( PressurePlate != nullptr )
	{
		PressurePlate->GetOverlappingActors( OUT OverlappingActors );
	}

	// Iterate through the actors and accumulating their mass
	for( auto &Actor : OverlappingActors )
	{
		if( !Actor->IsA<APawn>() )
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return TotalMass;
}