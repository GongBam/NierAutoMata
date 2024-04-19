// Fill out your copyright notice in the Description page of Project Settings.

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
	class AC_BossCharacter* boss;

	bool bPlayerDead = false;
	bool bBossDead = false;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UGameOver> gameover_BP;

	void ShowGameOverUI();
	void HideGameOverUI();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UGameStart> gameStartWidget_bp;	

protected:
	virtual void BeginPlay() override;

private:
	class UGameOver* gameOverUI;
	class UGameStart* gameStartUI;
};
