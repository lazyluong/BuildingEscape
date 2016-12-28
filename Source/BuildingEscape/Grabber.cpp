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

	UE_LOG( LogTemp , Warning , TEXT( "Grabber Component Initiated" ) );

	/// Look for attached Pyhsics Handle Component
	FindPhysicsHandleComponent();

	/// Look for attached Input Component
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime , TickType , ThisTickFunction );

	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation ,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation +
		PlayerViewPointRotation.Vector() * Reach;

	// If the phsyics handle is attached
	if( PhysicsHandle->GrabbedComponent )
	{
		// Move the object that we're holding
		PhysicsHandle->SetTargetLocation( LineTraceEnd );
	}
}



////////////////////////////////////////////////////////////
// Component Methods

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if( PhysicsHandle )
	{
		UE_LOG( LogTemp , Warning , TEXT( "%s'Physics Handle Component found." ) , *GetOwner()->GetName() );
	}
	else
	{
		UE_LOG( LogTemp , Error , TEXT( "%s's is missing input component." ) , *GetOwner()->GetName() );
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if( InputComponent )
	{
		UE_LOG( LogTemp , Warning , TEXT( "%s'Input Component found." ) , *GetOwner()->GetName() );

		/// Bind the input axis
		InputComponent->BindAction( "Grab" , IE_Pressed , this , &UGrabber::Grab );
		InputComponent->BindAction( "Grab" , IE_Released , this , &UGrabber::Release );
	}
	else
	{
		UE_LOG( LogTemp , Error , TEXT( "%s's is missing input component." ) , *GetOwner()->GetName() );
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation ,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation +
		PlayerViewPointRotation.Vector() * Reach;

	// Draw a red trace in the world to visualize
	DrawDebugLine( GetWorld() ,
		PlayerViewPointLocation , LineTraceEnd ,
		FColor::Red , false , 0.0f , 0 , 10.0f );

	/// Setup query parameters
	FCollisionQueryParams TraceParameters( FName( TEXT( "" ) ) , false , GetOwner() );

	/// Line-trace (A.K.A. Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT Hit , PlayerViewPointLocation , LineTraceEnd ,
		FCollisionObjectQueryParams( ECollisionChannel::ECC_PhysicsBody ) ,
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if( ActorHit )
	{
		UE_LOG( LogTemp , Warning , TEXT( "Line Tracing is hitting %s." ) , *ActorHit->GetName() );
	}

	return Hit;
}



////////////////////////////////////////////////////////////
// Input Methods

void UGrabber::Grab()
{
	UE_LOG( LogTemp , Warning , TEXT( "%s is performing a Grab Action." ) , *GetOwner()->GetName() );

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if( ActorHit )
	{
		// TODO: Attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab ,
			NAME_None ,
			ComponentToGrab->GetOwner()->GetActorLocation() ,
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG( LogTemp , Warning , TEXT( "%s is performing a Release Action." ) , *GetOwner()->GetName() );
	
	// Release physics handle
	PhysicsHandle->ReleaseComponent();
}

