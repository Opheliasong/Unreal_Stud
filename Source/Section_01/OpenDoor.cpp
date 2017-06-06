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
	OpenAngle = 10.0f;
	CloseAngle = 90.0f;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ʈ���� ������ �浹�ϸ�, �ش� �޼��带 ����
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();		
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}	
	
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}


void UOpenDoor::OpenDoor()
{
	//���� ȸ���� �����Ѵ�.
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	//���� ȸ���� �����Ѵ�.
	Owner->SetActorRotation(FRotator(0.f, CloseAngle, 0.f));
}