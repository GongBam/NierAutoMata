
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "PlayerHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UPlayerHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "PlayerSettings", meta = (BindWidget))
	class UProgressBar* playerHealthBar;

	void SetHealthBar(float value);

	class APlayerCharacter* playerCharacter;


};
