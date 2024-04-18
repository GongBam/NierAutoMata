
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossTeleportLocationActor.generated.h"

UCLASS()
class NIERPOD_API ABossTeleportLocationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossTeleportLocationActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UBoxComponent* boxComp;

};
