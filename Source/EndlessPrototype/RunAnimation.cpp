// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimation.h"
#include "GameFramework/PawnMovementComponent.h"

void URunAnimation::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn)) {
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Pawn && MovementComponent) {
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}
}
