// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST2BAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTEST2BAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	player = Cast<APlayerCharacter>(GetOwningActor());

	if(player != nullptr)
	{
		playerMovementComp = player ->GetCharacterMovement();
	}

}

void UTEST2BAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(playerMovementComp != nullptr)
	{
		bIsJump = playerMovementComp->IsFalling();
	}

}
