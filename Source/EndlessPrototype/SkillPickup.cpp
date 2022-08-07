// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillPickup.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include <EndlessPrototype/RunCharacter.h>

// Sets default values
ASkillPickup::ASkillPickup()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collider->SetupAttachment(SceneComponent);
	Collider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	SkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillMesh"));
	SkillMesh->SetupAttachment(Collider);
	SkillMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, 180, 0);

}

// Called when the game starts or when spawned
void ASkillPickup::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASkillPickup::OnSphereBeginOverlap);
	
}

void ASkillPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	
	if (RunCharacter) {
		RunCharacter->AddPickup();
		Destroy();
	}

}



