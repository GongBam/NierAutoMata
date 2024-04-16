// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerPodBullet.generated.h"

UCLASS()
class NIERPOD_API APlayerPodBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerPodBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "MySettings");
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float Speed = 5000;	

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USoundBase* explosionSound;

private:

	FVector moveDir;

	UFUNCTION()
	void OnoverlapEnermy(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp,
		int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
