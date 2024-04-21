
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
	float moveSpeed = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	int32 damage = 50;

	UPROPERTY(EditAnywhere, Category = "BossSettings|Sounds")
	class USoundBase* damagedSound; 

private:
	FVector moveDir;


	UFUNCTION()
	void OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
