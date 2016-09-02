// Karenne Sylvester, 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	

private:
	float Reach = 100.f; //how far ahead of the player we can reach
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	
	UInputComponent* InputComponent = nullptr;

	//Raycast and grab what's in reach
	void Grab();
	
	//Called when grab is released
	void Release();
	
	//Find attached physics handler
	void FindPhysicsHandleComponent();
	
	//Set up an assumed input component that is attached
	void SetUpInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	//returns current end of reach line
	FVector GetReachLineStart();
	
	//returns current end of reach line
	FVector GetReachLineEnd();
	
};
