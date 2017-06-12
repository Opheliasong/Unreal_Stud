// Fill out your copyright notice in the Description page of Project Settings.

#include "Section_01.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	//Physics Handle을 찾는다.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

//인풋 컴포넌트를 찾고, 액션이벤트의 Callback을 설정한다.
void UGrabber::SetupInputComponent()
{
	//Input Component를 찾는다.
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());
		return;
	}
	else
	{
		//Input axis를 대입시킨다.
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// 레이캐스트의 결과가 일정 거리보다 짧으면
	FHitResult Hit;

	//레이캐스트 쿼리 Parms를 설정한다.
	FCollisionQueryParams TraceParmeters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParmeters
	);
	return Hit;
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grab on"));
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	// Line trace의 Actor들의 Phyisics collision 채널을 설정을 시도하고, 찾는다.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	//해당 오브젝트를 보고 있는것으로 판정
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace Hit: %s"), *(ActorHit->GetName()));
		UPrimitiveComponent* PC = Cast<UPrimitiveComponent>(HitResult.GetActor()->GetRootComponent());

		UE_LOG(LogTemp, Warning, TEXT("Casting Done: %s"), *(ActorHit->GetName()));

		if (PC != NULL && PC->BodyInstance.bSimulatePhysics)
		{
			PhysicsHandle->GrabComponent(PC, NAME_None, PC->GetOwner()->GetActorLocation(), true);
		}
	}	
}

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grab release"));

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	PhysicsHandle->ReleaseComponent();	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	// 만약 물리적 오브젝트를 컨트롤 하고 있을 경우
	// 컨트롤 하는 오브젝트는 우리의 움직임에 따라 같이 움직인다.

	if (PhysicsHandle->GrabbedComponent != nullptr)
	{		
		//잡고 있는 오브젝트를 움직인다.
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}