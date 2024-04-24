
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NierGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API ANierGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	


public:
	

	class APlayerCharacter* player;
	class ABossCharacter* boss;

	bool bPlayerDead = false;
	bool bBossDead = false;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UGameOver> gameover_BP;

	void ShowGameOverUI();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UGameStart> gameStartWidget_bp;	

protected:
	virtual void BeginPlay() override;

private:
	class UGameOver* gameOverUI;
	class UGameStart* gameStartUI;
};
