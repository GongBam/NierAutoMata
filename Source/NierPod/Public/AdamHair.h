
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AdamHair.generated.h"

UCLASS()
class NIERPOD_API AAdamHair : public AActor
{
	GENERATED_BODY()
	
public:	
	AAdamHair();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BossSettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY()
	ABossCharacter* boss;

};
