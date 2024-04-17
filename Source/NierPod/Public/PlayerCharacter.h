
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
	//�÷��̾� ü��
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	float playerHealth = 1000.0f;

	//���� �ݸ��� �������� ������ 
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UPlayerHealthWidget* playerUI;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	TSubclassOf<class UPlayerHealthWidget> playerHealthWidget_bp;

	class ABossCharacter* bossCharacter;
	

protected:

	FVector moveDirection;


	//�̵�, �þ�, ���� �Լ� 
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

	//���� �Լ�
	void Dash(const FInputActionValue& InputValue);
	void DodgeFunction(const FInputActionValue& InputValue);
	void Shot(const FInputActionValue& InputValue);
	
	//void ResetDoubleJump();

	void SetActionState(EStateType eState);

	// Dodge
	void PerformDodge();
	void SaveDodge();

	//�ӽ÷� ���� �������ִ� �Լ�
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
	//��ǲ�׼�
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



	// �뽬
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DashAction;

	// ����
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* DodgeAction;

	class UAnimInstance* AnimInstance;

	bool canDash = true;
	FVector OldVelocity;
	FVector Velocity;
	float DashCooldown = 0.2f;

	int32 damage = 10;

	// �̴� ����
	bool DoubleJump;

	bool bSaveDodge;

	// ���� ����
	EStateType eActionState;
	int32 LightAttackIndex = 0;
	bool LightAttackSaved;

	// ������ ����
	int32 HeavyAttackIndex = 0;
	bool HeavyAttackSaved;

	// Dash ��Ÿ�� ���� ����
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DashMontage;

	// Dodge ��Ÿ�� ���� ����
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DodgeMontage;

	// Light Attack Combo ��Ÿ�� �迭 ����
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<class UAnimMontage*> lightAttackMontages;

	// Heavy Attack Combo ��Ÿ�� �迭 ����
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<class UAnimMontage*> heavyAttackMontages;
>>>>>>> Stashed changes
};