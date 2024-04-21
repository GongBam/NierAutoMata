
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class NIERPOD_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	AShield();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class USphereComponent* sphereComp2;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	int32 damage = 50;

	UPROPERTY(EditAnywhere, Category = "BossSettings|Sounds")
	class USoundBase* shieldAttackSound;

	class APlayerCharacter* player;

	UFUNCTION()
	void ShieldExtending();

	UFUNCTION()
	void ShieldAttacking(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SewwpResult);
};
