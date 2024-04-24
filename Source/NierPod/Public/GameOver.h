
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_restart;

	UPROPERTY(VisibleAnywhere, Category="MySettings", meta=(BindWidget))
	class UButton* btn_quit;

private:
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitGame();

};
