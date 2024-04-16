
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"


#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EStateType
{
	Nothing = 0,
	Attack,
	Dodge,
	EStateTypeMax
};

UCLASS()
class NIERPOD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

public:
	APlayerCharacter();

	//플레이어 데미지 입는 함수
	UFUNCTION()
	void PlayerDamaged();

	float GetCurrentHealth();
	float GetMaxHealth();

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UWidgetComponent* playerwidgetComp;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 currentHP;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 maxHP = 1000;



protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//플레이어 체력
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	float playerHealth = 1000.0f;

	//무기 콜리전 오버랩시 데미지 
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UPlayerHealthWidget* playerUI;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	TSubclassOf<class UPlayerHealthWidget> playerHealthWidget_bp;


protected:

	//이동, 시야, 점프 함수 
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();

	//공격 함수
	void Dash(const FInputActionValue& InputValue);
	void DodgeFunction(const FInputActionValue& InputValue);
	void Shot(const FInputActionValue& InputValue);
	
	//void ResetDoubleJump();

	void SetActionState(EStateType eState);

	// Dodge
	void PerformDodge();
	void SaveDodge();

	// LightAttack
	bool PerformLightAttack(int32 attackIndex);
	void ResetLightAttackVariables();

	// HeavyAttack
	bool PerformHeavyAttack(int32 attackIndex);
	void ResetHeavyAttackVariables();

	void PlayerDie();


	UFUNCTION()
	void LeftAttack(const FInputActionValue& InputValue);

	UFUNCTION()
	void RightAttack(const FInputActionValue& InputValue);

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	float walkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UStaticMeshComponent* weaponComp;

	//인풋액션맵핑
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	//인풋액션
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LeftAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* RightAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* shooting;




	// 대쉬
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DashAction;

	// 닷지
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DodgeAction;

	class UAnimInstance* AnimInstance;

	bool canDash = true;
	FVector OldVelocity;
	FVector Velocity;
	float DashCooldown = 0.2f;

	int32 damage = 10;

	// 이단 점프
	bool DoubleJump;

	// 구르기
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")

	bool bSaveDodge;

	// 왼쪽 공격
	EStateType eActionState;
	int32 LightAttackIndex = 0;
	bool LightAttackSaved;

	// 오른쪽 공격
	int32 HeavyAttackIndex = 0;
	bool HeavyAttackSaved;

	// Dash 몽타주 슬롯 생성
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DashMontage;

	// Dodge 몽타주 슬롯 생성
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DodgeMontage;

	// Light Attack Combo 몽타주 배열 생성
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<class UAnimMontage*> lightAttackMontages;

	// Heavy Attack Combo 몽타주 배열 생성
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<class UAnimMontage*> heavyAttackMontages;
};