
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossCharacter.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossSettings")
	class ABossCharacter* boss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossSettings")
	EBossState currentState;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private: 
	FTimerHandle deathTimer;
	
	UFUNCTION()
	void AnimNotify_AttackStarted();

	UFUNCTION()
	void AnimNotify_AttackEnded();

	UFUNCTION()
	void AnimNotify_Destroy();

	 UFUNCTION()
	 void AnimNotify_start();

	 UFUNCTION()
	 void AnimNotify_end();
	
	
};
