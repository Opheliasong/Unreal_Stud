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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 트리거 볼륨에 충돌하면, 해당 메서드를 실행
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}	
	else
	{
		CloseDoor();
	}
}


void UOpenDoor::OpenDoor()
{
	//1. 오너 엑터를 가져온다
	AActor* Owner = GetOwner();

	//2. 로테이터를 회전할 양만큼 설정하고
	FRotator NewRotation = FRotator(0.f, 10.f, 0.f);

	//3. 문의 회전을 설정한다.
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	//1. 오너 엑터를 가져온다
	AActor* Owner = GetOwner();

	//2. 로테이터를 회전할 양만큼 설정하고
	FRotator NewRotation = FRotator(0.f, 90.f, 0.f);

	//3. 문의 회전을 설정한다.
	Owner->SetActorRotation(NewRotation);
}