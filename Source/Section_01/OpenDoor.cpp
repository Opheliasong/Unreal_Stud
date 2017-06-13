// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_01.h"
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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 트리거 볼륨에 충돌하면, 해당 메서드를 실행
	//
	if (GetTotalMassOfActorOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float totMass = 0.0f;

	TArray<AActor*> OverlaapingActors;

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Not found Pressure plate"));
		return 0.f;
	}

	PressurePlate->GetOverlappingActors(OUT OverlaapingActors);

	//Range base for loop (since c++ 11)
	for (auto& iter : OverlaapingActors)
	{
		totMass += iter->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on Pressure plate"), *iter->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT("%f tot mass"), totMass);
	return totMass;
}