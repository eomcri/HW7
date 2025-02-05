

#include "SpartaPawn.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASpartaPawn::ASpartaPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	SpringArm->TargetArmLength = 300.0f;
	//SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetSkeletalMesh(MeshAsset.Object);
	}

	// set Physics false of CapsuleComponent and SkeletalMeshComponent
	SceneRoot->SetSimulatePhysics(false);
	Mesh->SetSimulatePhysics(false);

	SceneRoot->SetCapsuleHalfHeight(88.0f);
	SceneRoot->SetCapsuleRadius(34.0f);

	// add -90 Z rotation of mesh
	Mesh->SetRelativeRotation(FRotator(0,-90.0f,0));
	Mesh->SetRelativeLocation(FVector(0, 0, -88.0f));

	Speed = 5.0f;
	SprintSpeedMultiplier = 1.0f;
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaPawn::EndSprint
				);
			}
		}
	}
}

void ASpartaPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddActorLocalOffset(FVector(MoveInput.X * Speed * SprintSpeedMultiplier,0,0));
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(FVector(0, MoveInput.Y * Speed * SprintSpeedMultiplier, 0));
	}
}
void ASpartaPawn::StartJump(const FInputActionValue& value)
{
	if (!Controller) return;

}
void ASpartaPawn::StopJump(const FInputActionValue& value)
{
	if (!Controller) return;

}
void ASpartaPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;
	FVector2D LookInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0, LookInput.X, 0));
	}
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		FRotator NewRotation = SpringArm->GetRelativeRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y, -80.0f, 80.0f);
		SpringArm->SetRelativeRotation(NewRotation);
	}
}
void ASpartaPawn::StartSprint(const FInputActionValue& value)
{
	if (value.Get<bool>())
		SprintSpeedMultiplier = 2.0f;
}
void ASpartaPawn::EndSprint(const FInputActionValue& value)
{
	SprintSpeedMultiplier = 1.0f;
}

