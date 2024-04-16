// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"


/*
UENUM(BlueprintType)
enum class EAIState : uint8
{
	IDLE			UMETA(DisplayName = "Base State"),
	MOVE			UMETA(DisplayName = "Moving State"),
	ATTACK			UMETA(DisplayName = "Attack State"),
	ATTACKDELAY		UMETA(DisplayName = "Attack Delay State"),
};
*/


UCLASS()
class NIERPOD_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AAICharacter();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	/*
	UPROPERTY(EditAnywhere, Category = "MySettings")
	EAIState AIstate = EAIState::IDLE;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float traceSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDistance = 300.0f;

	float rotSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDelayTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float BossDistance = 700.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float PlayerDistance = 300.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float ThrowingDistance = 300.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class AWeaponActor> weapon_bp;
	*/

private:

	UFUNCTION()
	void OnAIAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class ABossCharacter* FindEnemy_Iterate();

	/*
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AIwalk;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AIAttack;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* AIThrowing;

	class AActor* target1;
	class AActor* target2;

	float currentTime = 0;

	void Idle(float DeltaSeconds);
	void Attack();
	void MoveToTarget(float DeltaSeconds);
	void AttackDelay(float DeltaSeconds);
	*/
};