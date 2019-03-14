// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "KartVehiclePawn.h"
#include "PawnStatComponent.h"

// Sets default values
ATriggerableItem::ATriggerableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Item mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);// make the vehicle into a root component

	// Trigger
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Component"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerableItem::OnOverlapBegin);
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ATriggerableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	if (OtherActor->IsA(AKartVehiclePawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item picked up"));

		AKartVehiclePawn* car = CastChecked<AKartVehiclePawn>(OtherActor);
		//car->PawnStatComponent->EnableMod(PawnStatMod);
		Destroy();
	}
}

