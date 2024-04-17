
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


protected:

	FVector moveDirection;


	//이동, 시야, 점프 함수 
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
	void PlayerDie();


	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = "PlayerSettings")
	class UStaticMeshComponent* weaponComp;

	
	UPROPERTY()
	class APlayerController* pc;
	class UTEST2BAnimInstance* playerAnim;

};