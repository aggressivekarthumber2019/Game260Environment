// Fill out your copyright notice in the Description page of Project Settings.

#include "KartVehiclePawn.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "DrawDebugHelpers.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "TimerManager.h"


// Sets default values
AKartVehiclePawn::AKartVehiclePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/////////////////////////
	// BOX COMPONENT SETUP //-------------------------------------------------------------------
	/////////////////////////
	
	// Mike: Create the box component for this class
	CarBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Root Box Collider"));

	// Mike: Set this box to be the root component
	SetRootComponent(CarBoxCollider);

	// Mike: Change the size of the box
	CarBoxCollider->SetBoxExtent(FVector(80.0f, 35.0f, 20.0f));

	// Mike: Enable physics for the box
	CarBoxCollider->SetSimulatePhysics(true);

	// Mike: Make a const reference to the custom collision channels made in the editor
	const ECollisionChannel Car_Body_Channel = ECC_GameTraceChannel1;
	const ECollisionChannel Car_Wheel_Channel = ECC_GameTraceChannel2;
	const ECollisionChannel Car_Mesh_Channel = ECC_GameTraceChannel3;

	// Mike: Setup the collision response of this car. It should be set to query and physics collisions mode
	CarBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	
	// Mike: Set set the collision channel of this box to be equal to the car body channel
	CarBoxCollider->SetCollisionObjectType(Car_Body_Channel);
	
	// Mike: Set the collision response to all channels to be block (it will hit anything)
	CarBoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	// Mike: Change it's response to two individual channels, ignore collisions with the wheel and mesh of the car
	CarBoxCollider->SetCollisionResponseToChannel(Car_Wheel_Channel, ECollisionResponse::ECR_Ignore);
	CarBoxCollider->SetCollisionResponseToChannel(Car_Mesh_Channel, ECollisionResponse::ECR_Ignore); 

	// Mike: DEBUG ONLY - Show the Box Collider in the game
	//CarBoxCollider->SetHiddenInGame(false);

	/////////////////////////
	// SKELETAL MESH SETUP //-------------------------------------------------------------------
	/////////////////////////
	
	// Mike: Create the USkeletalMeshComponent and give it a reasonable name
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vehicle SkeletalMesh"));
	
	// Mike: Attach this component to the root component 
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// Mike: Setup the collision responses of this class, it will ignore all collisions across the board
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetCollisionObjectType(Car_Mesh_Channel);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Mike: Add a small offset to the skeletal mesh so it is at the right level
	SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -35.0f));

	// Mike: Find the car mesh in the editor and set it to the CarMesh USkeletal Mesh Object
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/3D/Cars/GoodTestCar/JennyCar.JennyCar"));

	// Mike: Set the skeletal mesh of the skeletal mesh component 
	SkeletalMeshComponent->SetSkeletalMesh(CarMesh.Object);
	
	
	//////////////////////////
	// WHEEL COLLIDER SETUP //-------------------------------------------------------------------
	//////////////////////////

	// Mike: Setup the FRONT LEFT Wheel
	// Mike:Create default object
	FLWheelSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Front Left Wheel Collider"));
	
	// Mike: Add an offset so it's in the right location
	FLWheelSphereCollider->SetRelativeLocation(FVector(50.0f, -28.0f, -14.0f)); 
	
	// Mike: Set the size of the sphere
	FLWheelSphereCollider->SetSphereRadius(20.0f); 
	
	// Mike: Set the collision profile of this car to CarWheel
	FLWheelSphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FLWheelSphereCollider->SetCollisionObjectType(Car_Wheel_Channel);
	
	// Mike: Set the collisions response to all channels to be block
	FLWheelSphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	// Mike: Set the collisions response to the body and mesh challenge to ignore (this is the body of the car)
	FLWheelSphereCollider->SetCollisionResponseToChannel(Car_Body_Channel, ECollisionResponse::ECR_Ignore);
	FLWheelSphereCollider->SetCollisionResponseToChannel(Car_Mesh_Channel, ECollisionResponse::ECR_Ignore);
	
	// Mike: Attach to the skeletal Mesh
	FLWheelSphereCollider->SetupAttachment(CarBoxCollider);
	
	// Mike: DEBUG ONLY - Show the Box Collider in the game
	//FLWheelSphereCollider->SetHiddenInGame(false);

	// Mike: Setup the FRONT RIGHT Wheel - same parameters as the front left but the location is different
	FRWheelSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Front Right Wheel Collider"));
	FRWheelSphereCollider->SetRelativeLocation(FVector(50.0f, 28.0f, -14.0f));
	FRWheelSphereCollider->SetSphereRadius(20.0f);
	FRWheelSphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FRWheelSphereCollider->SetCollisionObjectType(Car_Wheel_Channel);
	FRWheelSphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	FRWheelSphereCollider->SetCollisionResponseToChannel(Car_Body_Channel, ECollisionResponse::ECR_Ignore);
	FRWheelSphereCollider->SetCollisionResponseToChannel(Car_Mesh_Channel, ECollisionResponse::ECR_Ignore);
	FRWheelSphereCollider->SetupAttachment(CarBoxCollider);
	//FRWheelSphereCollider->SetHiddenInGame(false);


	// Mike: Setup the BACK LEFT Wheel - same parameters as the front left but the location is different
	BLWheelSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Back Left Wheel Collider"));
	BLWheelSphereCollider->SetRelativeLocation(FVector(-52.0f, -31.0f, -14.0f));
	BLWheelSphereCollider->SetSphereRadius(20.0f);
	BLWheelSphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BLWheelSphereCollider->SetCollisionObjectType(Car_Wheel_Channel);
	BLWheelSphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BLWheelSphereCollider->SetCollisionResponseToChannel(Car_Body_Channel, ECollisionResponse::ECR_Ignore);
	BLWheelSphereCollider->SetCollisionResponseToChannel(Car_Mesh_Channel, ECollisionResponse::ECR_Ignore);
	BLWheelSphereCollider->SetupAttachment(CarBoxCollider);
	//BLWheelSphereCollider->SetHiddenInGame(false);

	// Mike: Setup the BACK RIGHT Wheel - same parameters as the front left but the location is different
	BRWheelSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Back Right Wheel Collider"));
	BRWheelSphereCollider->SetRelativeLocation(FVector(-52.0f, 31.0f, -14.0f));
	BRWheelSphereCollider->SetSphereRadius(20.0f);
	BRWheelSphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BRWheelSphereCollider->SetCollisionObjectType(Car_Wheel_Channel);
	BRWheelSphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BRWheelSphereCollider->SetCollisionResponseToChannel(Car_Body_Channel, ECollisionResponse::ECR_Ignore);
	BRWheelSphereCollider->SetCollisionResponseToChannel(Car_Mesh_Channel, ECollisionResponse::ECR_Ignore);
	BRWheelSphereCollider->SetupAttachment(CarBoxCollider);
	//BRWheelSphereCollider->SetHiddenInGame(false);


	/////////////////////////////
	// CAMERA SPRING ARM SETUP //-------------------------------------------------------------------
	/////////////////////////////

	// Sarfaraz: Create the default object of a spring arm component
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera SpringArm"));
	
	// Sarfaraz: Add an offset to the spring arm component, placing it behind the camera
	SpringArmCameraOffset = FVector(0.f, 0.f, 100.f);
	CameraSpringArm->TargetOffset = SpringArmCameraOffset;
	
	// Sarfaraz: Add a rotation to the spring arm component, making it face down a bit
	SpringArmCameraRotation = FRotator(-10.f, 0.f, 0.f); //Remove this line if you want to customize the values in blueprints
	CameraSpringArm->SetRelativeRotation(SpringArmCameraRotation);
	
	// Sarfaraz: Set the length of the arm
	CameraSpringArm->TargetArmLength = 600.0f;
	
	// Sarfaraz: Enable rotation lag with the camera
	CameraSpringArm->bEnableCameraRotationLag = true;
	
	// Sarfaraz: Set the rotation lag of the camera, making it slowly follow the vehicle instead of being rigidly attached to it
	CameraSpringArm->CameraRotationLagSpeed = 7.0f;
	
	// Sarfaraz: Set the arm to not inherit the pitch of the object it's following
	CameraSpringArm->bInheritPitch = false;
	
	// Sarfaraz: Set the arm to not inherit the roll of the object it's following
	CameraSpringArm->bInheritRoll = false;
	
	// Sarfaraz: Attach the camera to the root component of this object (the skeletal mesh)
	CameraSpringArm->SetupAttachment(CarBoxCollider);

	////////////////////
	//// CAMERA SETUP //-------------------------------------------------------------------
	////////////////////

	// Sarfaraz: Create the Camera component of the car and call it third person camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	
	// Sarfaraz: Attach the camera to the camera spring arm
	CameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	
	// Sarfaraz: Disable this camera from using the pawn control rotation
	CameraComponent->bUsePawnControlRotation = false;
	
	// Sarfaraz: Set the camera field of view
	ThirdPersonFOV = 90.0f; //REMOVE this if we want to set it at runtime / in blueprints
	CameraComponent->FieldOfView = ThirdPersonFOV;


	/////////////////////////
	//// SPEEDOMETER SETUP //-------------------------------------------------------------------
	/////////////////////////
	
	// Mike: Create a simple text renderer for the car speed
	CarSpeedText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Car Speed"));
	
	// Mike: Attach the text to the root component of the car
	CarSpeedText->SetupAttachment(RootComponent);

	// Mike: Rotate the text to face the camera
	CarSpeedText->SetWorldRotation(FRotator(0.0f, 180.0f, 0.0f));
	
	// Mike: Change the location of the text to face the camera
	CarSpeedText->SetRelativeLocation(FVector(-100.0f, -45.0f, 10.0f));


	/////////////////////////////////////
	//// PAWN MOVEMENT COMPONENT SETUP //-------------------------------------------------------------------
	/////////////////////////////////////

	// Sarfaraz: This is the movement component of the car, it does not need to be attached to the game object
	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Car Movement Component"));

	// Mike: Set the acceleation of the movement component
	PawnMovementComponent->Acceleration = VehiclePawnAcceleration;

	// Mike: Set the acceleation of the movement component
	PawnMovementComponent->Deceleration = VehiclePawnDeceleration;

	// Mike: Set the acceleation of the movement component
	PawnMovementComponent->MaxSpeed = VehiclePawnMaxSpeed;


	///////////////////////////////////
	//// DEFAULT VALUES FOR VEHICLES //-------------------------------------------------------------------
	///////////////////////////////////

	// Sarfaraz: Set the inputs current speed and turning rate to zero
	InputCurrentSpeedAmount = 0.0f;

	InputCurrenTurnAmount = 0.0f;

	// Sarfaraz: Setup the default values for the game's frame rate
	FrameRate = 0.0067f;
	canMove = true;
}

void AKartVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKartVehiclePawn::BeginPlay()
{
	// Mike: Call the parents begin play to ensure everything is initalized properly
	Super::BeginPlay();

	// Mike: DEBUG
	FString acceleration = FString::SanitizeFloat(PawnMovementComponent->Acceleration);
	FString deceleration = FString::SanitizeFloat(PawnMovementComponent->Deceleration);
	FString maxSpeed = FString::SanitizeFloat(PawnMovementComponent->MaxSpeed);

	// Mike: Debug Messages
	if (bShouldDisplayOnScreenDebug)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Purple, "Vehicle: Acceleration = " + acceleration);
	if (bShouldDisplayOnScreenDebug)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Purple, "Vehicle: Deceleration = " + deceleration);
	if (bShouldDisplayOnScreenDebug)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Purple, "Vehicle: Max Speed = " + maxSpeed);

	//This method will begin a timer which will run at at 60 frames per second regardless of the machine
	GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &AKartVehiclePawn::FixedUpdate, FrameRate, true);

	GetWorld()->GetTimerManager().SetTimer(LoopSpeedometer, this, &AKartVehiclePawn::UpdateSpeedometer, 0.1f, true);
}

void AKartVehiclePawn::FixedUpdate()
{
	// Sarfaraz: If the car is on the ground, then reduce the speed values and move the car
	//if (RayCastGround()) 
	{
		ReducedValues();
		MoveCar();
	}

	//Mike: Debug
	if (bShouldDisplayOnScreenDebug)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Red, "Turn Rate" + FString::SanitizeFloat(InputCurrenTurnAmount));
}

void AKartVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Sarfaraz: Called to bind functionality to input
	// Sarfaraz: Call the parent class's Setup Player Input Component
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Sarfaraz: Checking for Gameplay key bindings
	check(PlayerInputComponent);

	// Sarfaraz: Call the MoveX method with keys bind to 'MoveForward' 
	// Mike: This is currently bound to W (Forwards) and S (Backwards)
	// Sarfaraz: This is an axis so to get move backwards, simply add a key that adds a negative value to this method
	PlayerInputComponent->BindAxis("MoveForward", this, &AKartVehiclePawn::MoveX); 

	// Sarfaraz: call the MoveY method with keys bind to 'MoveRight'. 
	// Mike: This is currently bound to A (Left) and D (Right)
	// Sarfaraz: This is an axis so to get move left, simply add a key that adds a negative value to this method
	PlayerInputComponent->BindAxis("MoveRight", this, &AKartVehiclePawn::MoveY);  
	
	// Sarfaraz: Bind the method call "Boost Press" to the action "Boost" when the action is "Pressed"
	// Mike: This is currently bound to SHIFT
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AKartVehiclePawn::BoostPress);

	// Sarfaraz: Bind the method call "Boost Release" to the action "Boost" when the action is "Released"
	// Mike: This is currently bound to SHIFT
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &AKartVehiclePawn::BoostRelease);

	// Sarfaraz: Bind the method "Brake" to the axis "Brakes"
	// Mike: This is currently bound to CTRL
	PlayerInputComponent->BindAxis("Brakes", this, &AKartVehiclePawn::Brake);
}

void AKartVehiclePawn::UpdateSpeedometer()
{
	// Mike: Get the current speed of the car in INT form
	int speed_int = FMath::FloorToInt((PawnMovementComponent->Velocity.Size() / 50.0f));

	// Mike: Turn that int into a string
	SpeedometerString = FString::FromInt(speed_int);

	// Mike: Set the car speed text
	CarSpeedText->SetText(SpeedometerString);
}

void AKartVehiclePawn::BoostPress()
{
	//Debug Messages 
	if (bShouldDisplayOnScreenDebug)
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Blue, "Vehicle: Boost pressed");
	
	// Mike: Updated, now changing the max speed of the pawn
	PawnMovementComponent->MaxSpeed = VehiclePawnBoostSpeed;
}

void AKartVehiclePawn::BoostRelease()
{
	//Debug Messages 
	if (bShouldDisplayOnScreenDebug)
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Blue, "Vehicle: Boost Released");

	// Mike: Updated, now changing the max speed of the pawn
	PawnMovementComponent->MaxSpeed = VehiclePawnMaxSpeed;
}

void AKartVehiclePawn::MoveX(float AxisValue)
{
	// Mike: Set the current speed amount to be equal to the input from the keys
	InputCurrentSpeedAmount += AxisValue;

	// Mike: Clamp the speed amount so it won't accelerate too fast
	InputCurrentSpeedAmount = FMath::Clamp(InputCurrentSpeedAmount, InputMinSpeedAmount, InputMaxSpeedAmount);

	//Debug
	if (bShouldDisplayOnScreenDebug)
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Red, FString::SanitizeFloat(InputCurrentSpeedAmount));
}

void AKartVehiclePawn::MoveY(float AxisValue)
{
	// Sarfaraz: Movement on y direction
	InputCurrenTurnAmount += AxisValue;

	// Mike: Clamp the turning rate so that the car can't turn insanely fast
	InputCurrenTurnAmount = FMath::Clamp(InputCurrenTurnAmount, -InputTurnRateLimit, InputTurnRateLimit);
}

void AKartVehiclePawn::Brake(float AxisValue)
{
	// Sarfaraz: Slow the car down
	if(AxisValue > 0)
	{
		// Mike: Check if the car is almost at a full stop
		if (FMath::IsNearlyEqual(InputCurrentSpeedAmount, 0.0f, 0.05f))
		{
			//If the car is nearly at zero already, then simply stop the car
			InputCurrentSpeedAmount = 0;
		}

		else if(InputCurrentSpeedAmount > 0)
		{
			//Mike : Slow the car down with the break
			InputCurrentSpeedAmount -= BrakeSpeed;
		}

		else if(InputCurrentSpeedAmount < 0)
		{
			//Mike : Slow the car down with the break
			InputCurrentSpeedAmount += BrakeSpeed;
		}
		
		//Mike: DEBUG
		if (bShouldDisplayOnScreenDebug)
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 200.0f, FColor::Red, "Applying Breaks!");
	}	
}

void AKartVehiclePawn::ReducedValues()
{
	/////////////////////////
	// Mike: The following section focuses on getting the car to slow down to zero 
	// Sarfaraz: Slow the car down when it is not in motion
	if (FMath::IsNearlyZero(InputCurrentSpeedAmount, 0.005f))
	{
		InputCurrentSpeedAmount = 0;
	}
	// Mike: If the car speed is above zero, then slow it down
	else if (InputCurrentSpeedAmount > 0)
	{
		InputCurrentSpeedAmount -= SlowCarSpeedRate;
	}
	// Mike: If the car speed is below zero, then increase it to "get it closer to zero"
	else if (InputCurrentSpeedAmount < 0)
	{
		InputCurrentSpeedAmount += SlowCarSpeedRate;
	}

	//////////////////////////
	// Mike: The following section will be used to turn the wheel back to zero when the user is playing the game
	// Mike: If the car is nearly going straight, turn off the turning
	if (FMath::IsNearlyZero(InputCurrenTurnAmount, 0.005f))
	{
		InputCurrenTurnAmount = 0.0f;
	}
	// Mike: If the car is to the right, center it
	else if (InputCurrenTurnAmount > 0)
	{
		InputCurrenTurnAmount -= SlowCarTurnRate;
	}
	// Mike: If the car is to the left, center it
	else if (InputCurrenTurnAmount < 0)
	{
		InputCurrenTurnAmount += SlowCarTurnRate;
	}
}

void AKartVehiclePawn::MoveCar()
{
	// Sarfaraz: multiply the forward vector by the speed
	FVector TempFowardVector = FVector(GetActorForwardVector().X * InputCurrentSpeedAmount, GetActorForwardVector().Y * InputCurrentSpeedAmount, 0.0f);
	
	// Sarfaraz: Take the forward vector of the car and noramlize
	TempFowardVector.Normalize(1);

	// Sarfaraz: Add input to the car via add input vector
	PawnMovementComponent->AddInputVector(TempFowardVector);	

	// Sarfaraz: set the turn speed
	AddActorLocalRotation(FRotator(0, InputCurrenTurnAmount, 0));
}

bool AKartVehiclePawn::RayCastGround()
{
	// Sarfaraz: Make a hit result which will store what was hit by the trace
	FHitResult* hitResult = new FHitResult();

	// Sarfaraz: Get the up vector of the car, used for 
	FVector UpVector = GetActorUpVector();

	// Sarfaraz: The start of the trace will be at the cars location and below it
	FVector startTrace = GetActorLocation() - UpVector * 30;

	// Sarfaraz: The end of the trace will be a small line from the start trace and down
	FVector EndTrace = (-UpVector * 15.0f) + startTrace;

	// DEBUG ONLY Sarfaraz: Draw a debug line that represends the trace
	// DrawDebugLine(GetWorld(), startTrace, EndTrace, FColor::Red, true);

	// Sarfaraz: Line trace and see if anything is hit
	if (GetWorld()->LineTraceSingleByChannel(*hitResult, startTrace, EndTrace, ECC_Visibility))
	{
		// Sarfaraz: If something is hit, get the actor that was hit
		AActor* hitActor = hitResult->GetActor();

		// Sarfaraz: Check that the actor is valid and that it has the tag "Road". If it does, it's on the ground
		if (hitActor && hitActor->ActorHasTag("Road"))
		{			
			return true;
		}
		
	}
	return false;
}
