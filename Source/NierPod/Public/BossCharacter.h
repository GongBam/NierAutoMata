
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{	
	IDLE		UMETA(DisplayName = "Base State"),
	PLAYERKILL  UMETA(DisplayName = "Player Killed State"),
	MOVE		UMETA(DisplayName = "walk State"),
	ATTACKREADY UMETA(DisplayName = "Attack Ready State"),
	ROLLING		UMETA(DisplayName = "Rolling State"),
	ATTACK		UMETA(DisplayName = "Attack State"),
	ATTACK2     UMETA(DisplayName = "Attack2 State"),
	JUMPATTACK  UMETA(DisplayName = "Jump Attack State"),
	SHOOTATTACK UMETA(DisplayName = "Sooting Attack State"),
	ATTACKDELAY UMETA(DisplayName = "Attack Delay State"),
	BLOCK		UMETA(DisplayName = "Block State"),
	BLOCKATTACK UMETA(DisplayName = "Block Attack State"),
	DAMAGED		UMETA(DisplayName = "Damaged State"),
	PHASECHANGE UMETA(DisplayName = "Phase Changing State"),
	DIE			UMETA(DisplayName = "Dead State")
};

UCLASS()
class NIERPOD_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	EBossState bossState = EBossState::IDLE;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UCameraComponent* bossCamera; 

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	int32 maxHP = 5000;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float traceSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float attackDistance = 120.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float rollingDistance = 800.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float shootingTime = 6.0f;

	float rotSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float attackDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	int32 damage = 20;

	UFUNCTION(BlueprintCallable)
	void BoxCollisionExtending();

	UFUNCTION(BlueprintCallable)
	void BoxCollisionReset();

	UFUNCTION(BlueprintCallable)
	void PhaseChangeStart();

	UFUNCTION(BlueprintCallable)
	void PhaseChangeEnd();

	bool bPhaseChanging = false;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UAnimMontage* death_Montage;
	 
	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UAnimMontage* phaseChaingingMontage;

	UFUNCTION(BlueprintCallable)
	void OnDamaged(int32 dmg);

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UBoxComponent* leftFootCollision;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UBoxComponent* rightFootCollision;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class ADamageEffectActor* damageFX;

	UPROPERTY(EditAnywhere, Category = "BossSettings|Sounds")
	class USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "BossSettings|Sounds")
	class USoundBase* shieldSound;


	
	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult);

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	TSubclassOf<class AShield> shield;

	UPROPERTY()
	class AActor* target;

	UPROPERTY()
	class APlayerCharacter* playerCharacter;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	TSubclassOf<class UBossHealthWidget> bossHealthWidget_bp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UWidgetComponent* bosswidgetComp;

	class ABossTeleportLocationActor* teleportLoc;
	class ABossReturnLocationActor* returnLoc;
	class AShootingStartLocationActor* shootingLoc;
	class AShield* sh;


private:

	class UBossHealthWidget* bossUI;


	float currentTime = 0;
	

	int32 currentHP = 0;
	FVector hitLocation;

	bool bIsAttacked = false;
	bool shieldSpawn = false;
	bool bPhaseChanged = false;
	bool bShootingended = false;
	bool bPlayerAttacekd = false;
	bool bFirstShooting = false;
	bool bSecondShooting = false;


	void CheckDistance();


	//STATE 관련 함수 
	void Idle(float DeltaSeconds);
	void AttackReady();
	void Attack();
	void Attack2();
	void JumpAttack();
	void Rolling(float deltaSeconds);
	void MoveToTarget(float deltaSeconds);
	void AttackDelay(float deltaSeconds);
	void Blocking(float deltaSeconds);
	void BlocKAttack(float deltaSeconds);
	void ShootingAttack(float deltaSeconds);
	void DamageProcess(float deltaSeconds);
	void Phasing(float deltaSeconds);
	void Die();
	void PlayerKilled();


};
