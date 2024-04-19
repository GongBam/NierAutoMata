// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"
#include "NierGameModeBase.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// ����� ��ư�� Ŭ�� �̺�Ʈ�� RestartGame �Լ��� �����Ѵ�.
	btn_restart->OnClicked.AddDynamic(this, &UGameOver::RestartGame);

	// ���� ��ư�� Ŭ�� �̺�Ʈ�� QuitGame �Լ��� �����Ѵ�.
	btn_quit->OnClicked.AddDynamic(this, &UGameOver::QuitGame);

	btn_continue->OnClicked.AddDynamic(this, &UGameOver::ContinueGame);
}

void UGameOver::RestartGame()
{
	// Level�� �ٽ� ����.
	UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"));
}

void UGameOver::QuitGame()
{
	// �����Ѵ�.
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Type::Quit, true);
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}

void UGameOver::ContinueGame()
{
	// ���� ��忡 �ִ� HideGameOverUI() �Լ��� �����Ѵ�.
	ANierGameModeBase* gm = Cast<ANierGameModeBase>(GetWorld()->GetAuthGameMode());

	if (gm != nullptr)
	{
		gm->HideGameOverUI();
	}
}
