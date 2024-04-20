
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageEffectActor.generated.h"

UCLASS()
class NIERPOD_API ADamageEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageEffectActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "BossSettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category = "BossSettings")
	class UParticleSystemComponent* particleComp;

	void PlayFX();
	void EndFX();

};
