// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TEST2BAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UTEST2BAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FVector moveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bIsJump = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	int32 attackNum=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bIsAttack=false;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UPROPERTY()
	class APlayerCharacter* player;

	UPROPERTY()
	class UCharacterMovementComponent* playerMovementComp;


};
