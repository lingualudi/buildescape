// Karenne Sylvester, 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 90.0f;
	
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	float LastDoorOpenTime;

	//returns total mass in kilograms
	float UOpenDoor::GetTotalMassOfActorsOnPlate();

	AActor* Owner = nullptr; //The owning door

	
};
