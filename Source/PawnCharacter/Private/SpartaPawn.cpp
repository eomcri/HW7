

#include "SpartaPawn.h"

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
	SpringArm->bUsePawnControlRotation = true;

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
	
}

void ASpartaPawn::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

