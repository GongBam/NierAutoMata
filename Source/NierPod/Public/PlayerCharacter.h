
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class NIERPOD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UInputMappingContext* imc_KeyMap;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UInputAction* ia_move;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UInputAction* ia_jump;
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_shot;
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_look;


	UFUNCTION()
	void PlayerDamaged();
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UWidgetComponent* playerwidgetComp;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 currentHP;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 maxHP = 1000;
	//플레이어 체력
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	float playerHealth = 1000.0f;

	//무기 콜리전 오버랩시 데미지 
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UPlayerHealthWidget* playerUI;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	TSubclassOf<class UPlayerHealthWidget> playerHealthWidget_bp;

	class ABossCharacter* bossCharacter;
	

protected:

	FVector moveDirection;


	//이동, 시야, 점프 함수 
<<<<<<< Updated upstream
	UFUNCTION()
	void PlayerMove(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void PlayerJump(const FInputActionValue& Value);
	UFUNCTION()
	void PlayerJumpEnd(const FInputActionValue& Value);
	UFUNCTION()
	void Shot(const FInputActionValue& Value);
	UFUNCTION()
=======
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

	//임시로 보스 데미지주는 함수
	void DAMAGING(const FInputActionValue& InputValue);

	// LightAttack
	bool PerformLightAttack(int32 attackIndex);
	void ResetLightAttackVariables();

	// HeavyAttack
	bool PerformHeavyAttack(int32 attackIndex);
	void ResetHeavyAttackVariables();

>>>>>>> Stashed changes
	void PlayerDie();


	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UStaticMeshComponent* weaponComp;

	
	UPROPERTY()
	class APlayerController* pc;
	class UTEST2BAnimInstance* playerAnim;

<<<<<<< Updated upstream
=======
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

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* damaging;



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
>>>>>>> Stashed changes
};