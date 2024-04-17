
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossPhasingActor.generated.h"

UCLASS()
class NIERPOD_API ABossPhasingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossPhasingActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UStaticMeshComponent* meshComp;
};
