
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossController.generated.h"

UCLASS()
class NIERPOD_API ABossController : public AAIController
{
	GENERATED_BODY()
	


public:
	void BeginPlay() override;


private:
	class UNavigationSystemV1* NavArea;

	FVector RandomLocation;

public:

	UFUNCTION()
	void RandomPatrol();

};
