
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GangGongThrow.generated.h"

UCLASS()
class NIERPOD_API AGangGongThrow : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGangGongThrow();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float Speed = 1000;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere,Category = "MySettings")
	class USoundBase* explosionSound;

private:
	FVector moveDir;
	
	bool bShouldStopMoving;

	class ABossCharacter* FindEnemy_Iterate();

	UFUNCTION()
	void OnoverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp,
		int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
