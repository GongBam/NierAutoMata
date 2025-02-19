
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
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_damaging;
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_LeftAttack;
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_RightAttack;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UInputAction* ia_dodge;
	UPROPERTY(EditAnywhere, category = "MySettings|Inputs")
	class UInputAction* ia_SelfDamage;
	UPROPERTY(EditAnywhere, Category= "MySettings|Inputs")
	class UAnimMontage* Left_montages;

	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* Right_montages;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* Draw_montage;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* Die_montage;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* Dodge_montage;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* OnDamaged_montage;
	UPROPERTY(EditAnywhere, Category = "MySettings|Inputs")
	class UAnimMontage* steashe_montage;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APlayerDamageEffectActor* damageFX;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings|Sounds")
	class USoundBase* swordSound;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings|Sounds")
	class USoundBase* swordHitSound;
	

	//일반공격 데미지받는 함수
	UFUNCTION()
	void PlayerDamaged(int32 dmg);
	//데미지 받으면서 튕겨지는 함수 
	UFUNCTION()
	void PlayerDamagedWithKnockBack(int32 dmg, AActor* attacker);

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UWidgetComponent* playerwidgetComp;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 currentHP;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	int32 maxHP = 1000;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	float playerHealth = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	float knockBackDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerSettings")
	class UStaticMeshComponent* weaponComp;


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class UPlayerHealthWidget* playerUI;

	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	TSubclassOf<class UPlayerHealthWidget> playerHealthWidget_bp;

	class ABossCharacter* bossCharacter;
	
	//카메라시점 변경 함수 
	UFUNCTION()
	void SwitchCameraToBoss();
	UFUNCTION()
	void DodgeFinished();

	UPROPERTY(BlueprintReadWrite, Category = "MySettings")
	bool bIsDead = false;

private:

	FVector moveDirection;
	FTimerHandle AttackTimer;
	FTimerHandle SteasheTimer;
	

	//임시로 보스 데미지주는 함수
	UFUNCTION()
	void DAMAGING(const FInputActionValue& Value);
	//자학함수
	UFUNCTION()
	void SelfDamaging(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void PlayerMove(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerJump(const FInputActionValue& Value);
	UFUNCTION()
	void PlayerJumpEnd(const FInputActionValue& Value);
	UFUNCTION()
	void Shot(const FInputActionValue& Value);
	UFUNCTION()
	void LeftAttack(const FInputActionValue& Value);
	UFUNCTION()
	void RightAttack(const FInputActionValue& Value);
	UFUNCTION()
	void Dodge(const FInputActionValue& Value);



	UFUNCTION()
	void PlayerDie();
	UFUNCTION()
	void EndAttack();
	UFUNCTION()
	void Steashe();


	

	UPROPERTY()
	class APlayerController* pc;
	
	UPROPERTY()
	class UTEST2BAnimInstance* playerAnim;

	class ANierGameModeBase* nierGM;

	bool canDash = true;
	FVector OldVelocity;
	FVector Velocity;
	float DashCooldown = 0.2f;


	int32 damage = 0;
	int32 left = 1;
	int32 right = 1;


	// 이단 점프
	bool DoubleJump;
	bool DrawSword = false;
	bool Loca = false;
	bool isDodge=false;
	FVector PlayerLocation;

};