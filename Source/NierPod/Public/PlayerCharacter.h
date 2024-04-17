
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


	//�ӽ÷� ���� �������ִ� �Լ�
	void DAMAGING(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	void PlayerMove(const FInputActionValue& Value);
	void PlayerJump(const FInputActionValue& Value);
	void PlayerJumpEnd(const FInputActionValue& Value);
	void Shot(const FInputActionValue& Value);

	
	void PlayerDie();


	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UStaticMeshComponent* weaponComp;

	
	UPROPERTY()
	class APlayerController* pc;
	class UTEST2BAnimInstance* playerAnim;

	bool canDash = true;
	FVector OldVelocity;
	FVector Velocity;
	float DashCooldown = 0.2f;

	int32 damage = 10;

	// �̴� ����
	bool DoubleJump;

};