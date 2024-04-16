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
	virtual void Tick(float DeltaTime) override;

	class APlayerCharacter* player;
	class AC_BossCharacter* boss;

	bool bPlayerDead = false;
	bool bBossDead = false;


};
