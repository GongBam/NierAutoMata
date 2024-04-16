
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{	
	IDLE		UMETA(DisplayName = "Base State"),
	MOVE		UMETA(DisplayName = "walk State"),
	ATTACKREADY UMETA(DisplayName = "Attack Ready State"),
	ATTACK		UMETA(DisplayName = "Attack State"),
	ATTACK2     UMETA(DisplayName = "Attack2 State"),
	JUMPATTACK  UMETA(DisplayName = "Jump Attack State"),
	ATTACKDELAY UMETA(DisplayName = "Attack Delay State"),
	BLOCK		UMETA(DisplayName = "Block State"),
	BLOCKATTACK UMETA(DisplayName = "Block Attack State"),
	DAMAGED		UMETA(DisplayName = "Damaged State"),
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
	int32 maxHP = 1000;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float traceSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")

	float attackDistance = 300.0f;


	float rotSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float attackDelayTime = 3.0f;

	UFUNCTION(BlueprintCallable)
	void BoxCollisionExtending();

	UFUNCTION(BlueprintCallable)
	void BoxCollisionReset();

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UAnimMontage* death_Montage;

	void OnDamaged(int32 dmg);

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UBoxComponent* leftFootCollision;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UBoxComponent* rightFootCollision;
	
	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult);

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	TSubclassOf<class AShield> shield;

	UPROPERTY()
	class AActor* target;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	TSubclassOf<class UBossHealthWidget> bossHealthWidget_bp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UWidgetComponent* bosswidgetComp;

	double GetMaxHealth();
	double GetHealth();
private:

	class UBossHealthWidget* bossUI;


	float currentTime = 0;

	int32 currentHP = 0;
	FVector hitLocation;


	void Idle(float DeltaSeconds);


	void AttackReady();
	void Attack();
	void Attack2();
	void JumpAttack();

	void MoveToTarget(float deltaSeconds);
	void AttackDelay(float deltaSeconds);
	void Blocking(float deltaSeconds);
	void BlocKAttack();
	void DamageProcess(float deltaSeconds);
	void Die();


};
