

#include "PlayerHealthWidget.h"
#include "PlayerCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

void UPlayerHealthWidget::NativeTick(const FGeometry& PlayerGeometry, float DeltaSeconds)
{

	if (playerCharacter != nullptr)
	{
		playerHealthBar->SetPercent(playerCharacter->GetCurrentHealth() / playerCharacter->GetMaxHealth());
	}

}
