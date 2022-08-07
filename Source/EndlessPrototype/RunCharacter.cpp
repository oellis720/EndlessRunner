// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "GameFramework/SpringarmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Obstacle.h"
#include "EndlessPrototypeGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating spring arm and setting defaults
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.0f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.0f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	//create camera and attach to arm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	//ability defaults
	isUsingAbility = false;
	AbilityDuration = 5.0f;

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessPrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//crucial; if fail, something is wrong
	check(RunGameMode);
}


// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto running
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Roll = 0.f;
	ControlRotator.Pitch = 0.f;

	AddMovementInput(ControlRotator.Vector());
	GetCharacterMovement()->MaxWalkSpeed += 0.25f;

}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//binding movement
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARunCharacter::MoveDown);

	//bind jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);

	//bind ability
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ARunCharacter::UseAbility);
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	static FVector Impulse = FVector(0, 0, MoveDownImpulse);

	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinish()
{
	CurrentLane = NextLane;
}

void ARunCharacter::Death()
{
	if (!bIsDead) {
		UWorld* World = GetWorld();

		if(World) {
			bIsDead = true;
			DisableInput(nullptr);
			GetCharacterMovement()->DisableMovement();

			GetMesh()->SetVisibility(false);
			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.0f);
			
		}
	}
}

void ARunCharacter::OnDeath()
{
	GameOver();
	if (RestartTimerHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}
}

void ARunCharacter::GameOver()
{
	if (GameOverScreenClass) {
		UUserWidget* Widget = CreateWidget(GetWorld(), GameOverScreenClass);
		APlayerController* Mouse = Cast<APlayerController>(GetController());

		if (Widget) {
			Widget->AddToViewport();
		}

		if (Mouse) {
			Mouse->bShowMouseCursor = true;
		}
	}


}

void ARunCharacter::AddPickup()
{
	RunGameMode->AddPickup();
	PickupCount = RunGameMode->TotalPickedUp;
}

void ARunCharacter::UseAbility()
{
	if (PickupCount >= 10) {
		if (!isUsingAbility) {
			isUsingAbility = true;
			RunGameMode->TotalPickedUp = 0;
			PickupCount = RunGameMode->TotalPickedUp;

			if (auto SpeedMovement = GetCharacterMovement()) {
				SpeedMovement->MaxWalkSpeed *= 2;
			}
			GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARunCharacter::OnBoxHit);
			GetWorld()->GetTimerManager().SetTimer(AbilityTimerHandle, this, &ARunCharacter::ResetAbility, AbilityDuration, false);
		}

	}
}

void ARunCharacter::ResetAbility()
{
	GetCapsuleComponent()->OnComponentHit.Clear();
	if (auto SpeedMovement = GetCharacterMovement()) {
		SpeedMovement->MaxWalkSpeed /= 2;
	}
	isUsingAbility = false;
}

void ARunCharacter::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AObstacle* Box = Cast<AObstacle>(OtherActor);

	if (Box) {
		Box->Destroy();
	}

}