// Copyright Luong Chuong 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

////////////////////////////////////////////////////////////
// Constructor

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



////////////////////////////////////////////////////////////
// Override Methods

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG( LogTemp , Warning , 
		TEXT( "%s's Grabber Component Initiated." ) ,
		*GetOwner()->GetName() );

	/// Look for attached Pyhsics Handle Component
	FindPhysicsHandleComponent();

	/// Look for attached Input Component
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	// If the phsyics handle is attached
	if( PhysicsHandle->GrabbedComponent != nullptr )
	{
		// Move the object that we're holding
		PhysicsHandle->SetTargetLocation( GetReachLineEnd() );
	}
}



////////////////////////////////////////////////////////////
// Component Methods

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle != nullptr )
	{
		UE_LOG( LogTemp , Warning , 
			TEXT( "%s's Physics Handle Component found." ) , 
			*GetOwner()->GetName() );
	}
	else
	{
		UE_LOG( LogTemp , Error , 
			TEXT( "%s's is missing input component." ) , 
			*GetOwner()->GetName() );
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if( InputComponent != nullptr )
	{
		UE_LOG( LogTemp , Warning , 
			TEXT( "%s's Input Component found." ) , 
			*GetOwner()->GetName() );

		/// Bind the input axis
		InputComponent->BindAction( "Grab" , IE_Pressed , this , &UGrabber::Grab );
		InputComponent->BindAction( "Grab" , IE_Released , this , &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp , Error , 
			TEXT( "%s's is missing input component." ) , 
			*GetOwner()->GetName() );
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ) , false , GetOwner() );

	/// Line-trace (A.K.A. Ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitResult , GetReachLineStart() , GetReachLineEnd() ,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ) ,
		TraceParameters
	);

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation ,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation ,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}



////////////////////////////////////////////////////////////
// Input Methods

void UGrabber::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if( ActorHit )
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab ,
			NAME_None , // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation() ,
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	// Release physics handle
	PhysicsHandle->ReleaseComponent();
}
