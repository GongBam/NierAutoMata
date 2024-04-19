// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStart.generated.h"

/**
 * 
 */
UCLASS()
class NIERPOD_API UGameStart : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void EnableUserInput(bool bEnable);
};
