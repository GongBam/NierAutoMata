
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossCharacter.h"
#include "BossHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UBossHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& BossGeometry, float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere, Category = "BossSettings", meta=(BindWidget))
	class UProgressBar* BossHealthBar;

	class ABossCharacter* bossCharacter;



};
