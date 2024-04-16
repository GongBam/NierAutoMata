

#include "BossController.h"
#include "NavigationSystem.h"

void ABossController::BeginPlay()
{
	Super::BeginPlay();

	//랜덤으로 돌아다닐 전체범위
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	RandomPatrol();

}

void ABossController::RandomPatrol()
{	
	if (GetPawn() != nullptr)
	{
		//정해진 범위 내에서 닿을 수 있는 랜덤한 위치로 감
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 15000.0f);

		MoveToLocation(RandomLocation);
	}
}
