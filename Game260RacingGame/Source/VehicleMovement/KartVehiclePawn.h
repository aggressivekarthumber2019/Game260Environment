// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KartVehiclePawn.generated.h"

/**
* Blueprintable: You can make blueprints out of this class
*
* BlueprintType: This class is accessible in blueprints
*/
 
UCLASS(BlueprintType, Blueprintable)
class VEHICLEMOVEMENT_API AKartVehiclePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKartVehiclePawn();

	/////////////////////////////////////////////////
	///// DEBUGGING VARIABLES AND METHODS ///////////
	/////////////////////////////////////////////////

	/** DEBUG MODE - If this is true, it will display on screen debug messages*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Parts | Debug")
	bool bShouldDisplayOnScreenDebug = true;

	UFUNCTION(BlueprintCallable, Category = "Vehicle Parts | Debug")
	void SetDebugModeEnabled(bool bDebugOn) { bShouldDisplayOnScreenDebug = bDebugOn; };

	////////////////////////////////////////////////
	////// VEHICLE COMPONENTS //////////////////////
	////////////////////////////////////////////////

	/** Sarfaraz: Vehicle with a mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
	class USkeletalMeshComponent* SkeletalMeshComponent;

	/** Mike: Box Collider that represents the body of the car */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
	class UBoxComponent* CarBoxCollider;

	/** Mike: Sphere component for the front left wheel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
	class USphereComponent* FLWheelSphereCollider;

	/** Mike: Sphere component for the front right wheel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
		class USphereComponent* FRWheelSphereCollider;

	/** Mike: Sphere component for the back left wheel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
		class USphereComponent* BLWheelSphereCollider;

	/** Mike: Sphere component for the back right wheel */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Skeleton")
		class USphereComponent* BRWheelSphereCollider;

	/** Sarfaraz: Vehicle Movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Movement Component")
	class UFloatingPawnMovement* PawnMovementComponent;

	/** Mike: The floating pawn movements acceleration value. Can be changed in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float VehiclePawnAcceleration;

	/** Mike: The floating pawn movements deceleration value. Can be changed in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float VehiclePawnDeceleration;

	/** Mike: The floating pawn movements max speed value. Can be changed in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float VehiclePawnMaxSpeed;

	/** Mike: The floating pawn movements max speed value. Can be changed in blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float VehiclePawnBoostSpeed;

	/** Text component for the In-Car speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Movement Component")
	class UTextRenderComponent* CarSpeedText;

	///////////////////////////////////////
	//////// CAMERA COMPONENTS ////////////
	///////////////////////////////////////

	/** Sarfaraz: Camera component of the car */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Camera")
	class UCameraComponent* CameraComponent;

	/** Sarfaraz: Controls camera delay and movement behind the car */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Camera")
	class USpringArmComponent* CameraSpringArm;

	/** Mike: The public offset of the camera that can be set in the editor. Could also be used for changing the camera angle while playing the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Camera")
	FVector SpringArmCameraOffset;

	/** Mike: The public rotation of the camera that can be set in the editor. Could also be used for changing the camera rotation while playing the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Camera")
	FRotator SpringArmCameraRotation;

	/** Mike: The public Field of View of the third person camera. Could also be used for changing the FOV while playing the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Camera")
	float ThirdPersonFOV;


	//////////////////////////////////////////////
	////// INPUT VARIABLES  //////////////////////
	/////////////////////////////////////////////

	/** Mike: This is the minimum amount of input that can be applied to the cars speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float InputMinSpeedAmount;

	/** Mike: This is the maximum amount of input that can be applied to the cars speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float InputMaxSpeedAmount;

	/** Mike: This is the max turning rate for the car */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float InputTurnRateLimit;

	/** Mike: This is the braking speed of the car */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float BrakeSpeed;

	/** Mike: This is the braking speed of the car */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float SlowCarSpeedRate;

	/** Mike: This is the braking speed of the car */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle Parts | Car Stats")
	float SlowCarTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Parts")
	float FrameRate;

	// Timer function
	FTimerHandle LoopTimerHandle;
	FTimerHandle LoopSpeedometer;
	
	/** Mike: Setter function for current speed */
	UFUNCTION(BlueprintCallable, Category = "Vehicle Parts | Setter")
	void SetCurrentSpeedAmount(float val) { InputCurrentSpeedAmount = val; }

	/** Mike: Setter function for current turn amount */
	UFUNCTION(BlueprintCallable, Category = "Vehicle Parts | Setter")
	void SetCurrentTurnAmount(float val) { InputCurrenTurnAmount = val; }

	/** Mike: Getter function for current speed */
	UFUNCTION(BlueprintCallable, Category = "Vehicle Parts | Getter")
	float GetCurrentSpeedAmount() { return InputCurrentSpeedAmount;  }

	/** Mike: Getter function for current turn amount */
	UFUNCTION(BlueprintCallable, Category = "Vehicle Parts | Getter")
	float GetCurrentTurnAmount() { return InputCurrenTurnAmount; }

private:

	// Mike: String that displays the speed of the car
	FString SpeedometerString;

	// Mike: The current speed and turning rate of the car
	float InputCurrentSpeedAmount;
	float InputCurrenTurnAmount;

	// Sarfaraz: Boolean to determine if the car can move or not
	bool  canMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Mike: Set Speedometer
	void UpdateSpeedometer();

	// Movement on x and y direction method
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

	/** Sarfaraz: Called when the user breaks */
	void Brake(float AxisValue);

	//Raycast to check if the car is on the ground
	bool RayCastGround();

	//Method used to move the car forward
	void MoveCar();

	/** Sarfaraz: This method is used to slow the car down when the accelerator is not pressed and to straighten the wheels */
	void ReducedValues();

	/** Sarfaraz: When the user presses the boost key, this method is called */
	void BoostPress();

	/** Sarfaraz: When the user releases the boost, this method is called */
	void BoostRelease();

	/** Sarfaraz: This method is called on a timer at a constant rate*/
	void FixedUpdate();

};