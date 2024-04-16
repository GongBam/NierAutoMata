
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
	
protected:
	virtual void NativeTick(const FGeometry& PlayerGeometry, float DeltaSeconds) override;

public:

	UPROPERTY(EditAnywhere, Category = "PlayerSettings", meta = (BindWidget))
	class UProgressBar* playerHealthBar;

	class APlayerCharacter* playerCharacter;


};
