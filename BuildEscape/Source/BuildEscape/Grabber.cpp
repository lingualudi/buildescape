// Karenne Sylvester, 2016

#include "BuildEscape.h"
#include "Grabber.h"
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetUpInputComponent();
	
}

///Look for the attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
	
}

//Look for the attached input component
void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found."))
			//Bind the input action
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
			InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}


void UGrabber::Grab()
{
	//Raycast and grab

		///LINE TRACE, only when grabber grabbed ...try and reach actors with physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();//gets the mesh we're moving
		auto ActorHit = HitResult.GetActor();

		///if we hit something, then attach a physics handle
		if (ActorHit)
		{
			if (!PhysicsHandle)
			{
				return;
			}
			//attach physics handle
			PhysicsHandle->GrabComponent(
				ComponentToGrab, //mesh
				NAME_None,//no bones needed because not skeletal
				ComponentToGrab->GetOwner()->GetActorLocation(),
				true //allow rotation
			);
		}
}

void UGrabber::Release()
{
	//Let go of what was grabbed

	if (!PhysicsHandle)
	{
		return;
	}
		///release physics handle
		PhysicsHandle->ReleaseComponent();
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	
	///Ray-cast out to reach distance - will need a private
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, 
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters
	);

	
	return HitResult;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
	{
		return;
	}
	///if the physics handle is attached, 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move object that is held
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

						// OLD CODE, Leave in because useful to view/re-use for debugging purposes...
						//Logout to test
						/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
							*PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString())*/

							///Draw a red trace in the world to visualise
							/*DrawDebugLine(
								GetWorld(),
								PlayerViewpointLocation,
								LineTraceEnd,
								FColor(255, 0, 0),
								false,
								0.f,
								0.f,
								10.f
							);*/

}

FVector UGrabber::GetReachLineStart()
{
	///Get the player viewpoint every tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;
}
	
FVector UGrabber::GetReachLineEnd()
{
	///Get the player viewpoint every tick
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}
		
	
	