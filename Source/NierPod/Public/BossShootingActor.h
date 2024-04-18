
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossShootingActor.generated.h"

UCLASS()
class NIERPOD_API ABossShootingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossShootingActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	float moveSpeed = 600.0f;

private:
	FVector moveDir;


	UFUNCTION()
	void OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
