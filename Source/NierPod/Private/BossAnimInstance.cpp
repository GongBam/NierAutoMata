

#include "BossAnimInstance.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// 보스캐릭터 가져오기
	boss = Cast<ABossCharacter>(GetOwningActor());


}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (boss != nullptr)
	{	//보스의 Enum BossState 를 가져옴 
		currentState = boss->bossState;	

	}
}

void UBossAnimInstance::AnimNotify_AttackStarted()
{	//어택시작시 발 공격콜리전 크기 커짐
	boss->BoxCollisionExtending();
}

void UBossAnimInstance::AnimNotify_AttackEnded()
{	//어택 끝나면 발 공격콜리전 크기 초기화
	boss->BoxCollisionReset();

}

//죽는 애니메이션 후 5초 뒤 보스 사라짐
void UBossAnimInstance::AnimNotify_Destroy()
{
	if (boss != nullptr && !GetWorld()->GetTimerManager().IsTimerActive(deathTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([&]() {

		boss->Destroy();
			}), 5.0f, false);
	}
}

void UBossAnimInstance::AnimNotify_start()
{
	boss->PhaseChangeStart();
}

void UBossAnimInstance::AnimNotify_end()
{
	boss->PhaseChangeEnd();
}
