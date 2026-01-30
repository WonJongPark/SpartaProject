// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyItem.h"

AKeyItem::AKeyItem()
{
	ItemType = "Key";
}

void AKeyItem::OnItemOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Key Collected!"));
}

void AKeyItem::ActivateItem(AActor* Activator)
{
	UE_LOG(LogTemp, Warning, TEXT("Key used to unlock the door!"));
}
