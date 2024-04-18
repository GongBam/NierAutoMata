
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingStartLocationActor.generated.h"

UCLASS()
class NIERPOD_API AShootingStartLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AShootingStartLocationActor();

protected:
	virtual void BeginPlay() override;
	

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="BossSettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category="BossSettings")
	class UArrowComponent* arrowComp;

	UPROPERTY(EditAnywhere, Category="BossSettings")
	float delayTime = 0.7f;

	UPROPERTY(EditAnywhere, Category="BossSettings")
	TSubclassOf<class ABossShootingActor> shooting_bp;

	bool bShoting = false;


private:
	float currentTime = 0;
};
