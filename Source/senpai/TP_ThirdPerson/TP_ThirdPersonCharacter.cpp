// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;
	bIsDashing = false;
	bIsDead = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("DetectGamepad",IE_Pressed,this,&ATP_ThirdPersonCharacter::DetectGamepad);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATP_ThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATP_ThirdPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnX", this, &ATP_ThirdPersonCharacter::TurnX);
	PlayerInputComponent->BindAxis("TurnY", this, &ATP_ThirdPersonCharacter::TurnY);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	// PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}


void ATP_ThirdPersonCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector Direction = FollowCamera->GetForwardVector();
		Direction.Z = 0;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector Direction = FollowCamera->GetRightVector();
		Direction.Z = 0;
		Direction.Normalize();
		AddMovementInput(Direction, Value);
	}
}

void ATP_ThirdPersonCharacter::TurnX(float Value)
{
	//if (Value == 0) return;
	GamePadLookAtDirection.X = Value;
	//UE_LOG(LogTemp, Log, TEXT("X: %f"), Value);
}

void ATP_ThirdPersonCharacter::TurnY(float Value)
{
	//if (Value == 0) return;
	GamePadLookAtDirection.Y = Value;
	//UE_LOG(LogTemp, Log, TEXT("Y: %f"), Value);
}

void ATP_ThirdPersonCharacter::DetectGamepad(FKey key)
{
	if(key.IsGamepadKey())
	{
		bIsUsingGamepad = true;
	}
	else
		bIsUsingGamepad = false;
	//UE_LOG(LogTemp,Warning,TEXT("%i"),bIsUsingGamepad);
}

void ATP_ThirdPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsDead) {
		return;
	}

	if (!bIsUsingGamepad)
	{
		//FHitResult hit;
		FVector WorldOrigin;
		FVector WorldDirection;
		FVector2D ScreenPosition;
		//FCollisionQueryParams CollisionQueryParams;
		//CollisionQueryParams.AddIgnoredActor(this);
		const APlayerController* controller = Cast<APlayerController>(GetController());
		controller->GetMousePosition(ScreenPosition.X,ScreenPosition.Y);

		float T = 0;
		FVector Intersection;
		bool didIntersect = false;

		if (UGameplayStatics::DeprojectScreenToWorld(controller, ScreenPosition, WorldOrigin, WorldDirection) == true)
		{
			didIntersect = UKismetMathLibrary::LinePlaneIntersection_OriginNormal(WorldOrigin, WorldOrigin + WorldDirection * CameraBoom->TargetArmLength*8, GetActorLocation(),FVector::UpVector,T,Intersection);

			//GetWorld()->LineTraceSingleByChannel(hit, WorldOrigin, WorldOrigin + WorldDirection * CameraBoom->TargetArmLength*8, ECC_WorldStatic, CollisionQueryParams);
		}
		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_WorldStatic, true, hit);

		if(!didIntersect){return;}

		FVector location = Intersection;
		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), location);
		this->customRotation = lookAt;
		//DrawDebugSphere(GetWorld(),location,50,32,FColor::Red);
		if (!bIsDashing) {
			SetActorRotation(FRotator(0, lookAt.Yaw, 0));
		}
	}
	else
	{
		if (GamePadLookAtDirection.Length() != 0)
		{
			FVector forward = FollowCamera->GetForwardVector();
			forward.Z = 0;
			forward.Normalize();
			FVector right = FollowCamera->GetRightVector();
			right.Z = 0;
			right.Normalize();
			FVector CameraRelativeLocation = forward * -GamePadLookAtDirection.Y + right * GamePadLookAtDirection.X;
			FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			                                                         GetActorLocation() + CameraRelativeLocation);
			
			// UE_LOG(LogTemp, Warning, TEXT("relative vector: %f %f %f"), CameraRelativeLocation.X,
			//        CameraRelativeLocation.Y, CameraRelativeLocation.Z)
			if (!bIsDashing) {
				SetActorRotation(FRotator(0, lookAt.Yaw, 0));
			}
			
		}
		else
		{
			//rotate towards movement?
		}
		this->customRotation = GetActorRotation();
	}
}
