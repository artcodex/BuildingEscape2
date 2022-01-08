// Copyright Michael Bridges 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	float initialYaw = 0.f;
	float targetYaw = 0.f;	

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float openYaw = 90.f;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;

	UPROPERTY(EditAnywhere)
	float openSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	float closeSpeed = 2.f;

	float currentSpeed = openSpeed;

	float doorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float doorCloseDelay = 2.f;

private: 
	void SwingDoor(float DeltaTime);
	APlayerController *GetPlayerController();
};
