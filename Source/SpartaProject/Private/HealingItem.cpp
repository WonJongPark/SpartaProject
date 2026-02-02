#include "HealingItem.h"

#include "SpartaCharacter.h"
#include "SpartaGameState.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
			if (UWorld* World = GetWorld())
			{
				if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
				{
					GameState->UpdateHUD();
				}
			}
		}
		
		DestroyItem();
	}
}
