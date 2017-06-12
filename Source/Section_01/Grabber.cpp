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
	//Physics Handle�� ã�´�.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

//��ǲ ������Ʈ�� ã��, �׼��̺�Ʈ�� Callback�� �����Ѵ�.
void UGrabber::SetupInputComponent()
{
	//Input Component�� ã�´�.
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());
		return;
	}
	else
	{
		//Input axis�� ���Խ�Ų��.
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// ����ĳ��Ʈ�� ����� ���� �Ÿ����� ª����
	FHitResult Hit;

	//����ĳ��Ʈ ���� Parms�� �����Ѵ�.
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

	// Line trace�� Actor���� Phyisics collision ä���� ������ �õ��ϰ�, ã�´�.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	//�ش� ������Ʈ�� ���� �ִ°����� ����
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

	// ���� ������ ������Ʈ�� ��Ʈ�� �ϰ� ���� ���
	// ��Ʈ�� �ϴ� ������Ʈ�� �츮�� �����ӿ� ���� ���� �����δ�.

	if (PhysicsHandle->GrabbedComponent != nullptr)
	{		
		//��� �ִ� ������Ʈ�� �����δ�.
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