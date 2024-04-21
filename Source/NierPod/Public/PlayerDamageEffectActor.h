
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerDamageEffectActor.generated.h"

UCLASS()
class NIERPOD_API APlayerDamageEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerDamageEffectActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class UParticleSystemComponent* particleComp;

	void PlayFX();
	void EndFX();


};
