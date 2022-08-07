// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTiles.h"
#include "Components/BoxComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <EndlessPrototype/RunCharacter.h>
#include <EndlessPrototype/SkillPickup.h>
#include "EndlessPrototypeGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFloorTiles::AFloorTiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;
	
	//attach all components to the root/scene component
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterLane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTrigger"));
	FloorTrigger->SetupAttachment(SceneComponent);
	FloorTrigger->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f)); //setting defaults, can be changed later
	FloorTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AFloorTiles::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AEndlessPrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//crucial; if fail, something is wrong
	check(RunGameMode);

	FloorTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTiles::OnTriggerBoxOverlap);
	
}

// Called every frame
void AFloorTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFloorTiles::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter) {
		RunGameMode->AddTiles(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTiles::DestroyTiles, 2.0f, false);
	}
}

void AFloorTiles::DestroyTiles()
{
	if (DestroyHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	this->Destroy();

}

void AFloorTiles::SpawnItem()
{
	if (IsValid(SmallObstacleClass) && IsValid(LargeObstacleClass) && IsValid(SkillItemClass)) {
		
		int NumLarge = 0;
		SpawnLaneItem(CenterLane, NumLarge);
		SpawnLaneItem(LeftLane, NumLarge);
		SpawnLaneItem(RightLane, NumLarge);
	}
}

void AFloorTiles::SpawnLaneItem(UArrowComponent* Lane, int& NumLarge)
{
	const float RandomNum = FMath::FRandRange(0.f, 1.0f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	if (UKismetMathLibrary::InRange_FloatFloat(RandomNum, SpawnPercent1, SpawnPercent2, true, true)) {
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandomNum, SpawnPercent2, SpawnPercent3, true, true)) {
		if (NumLarge < 2) {
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(LargeObstacleClass, SpawnLocation, SpawnParameters);
			if (Obstacle) {
				NumLarge += 1;
			}
		}
		else {
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
		}
		
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandomNum, SpawnPercent3, 1.0f, true, true)) {
		ASkillPickup* SkillItem = GetWorld()->SpawnActor<ASkillPickup>(SkillItemClass, SpawnLocation, SpawnParameters);
	}
}