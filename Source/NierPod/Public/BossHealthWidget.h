
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


public:

	UPROPERTY(EditAnywhere, Category = "BossSettings", meta=(BindWidget))
	class UProgressBar* BossHealthBar;

	void SetHealthBar(float value);

	class ABossCharacter* bossCharacter;



};
