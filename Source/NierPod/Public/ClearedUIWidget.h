
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearedUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UClearedUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_restart;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_quit;

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();

	
};
