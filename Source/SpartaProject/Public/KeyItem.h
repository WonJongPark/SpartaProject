#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "KeyItem.generated.h"

UCLASS()
class SPARTAPROJECT_API AKeyItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AKeyItem();
	
	virtual void OnItemOverlap(
			UPrimitiveComponent* OverlapComp,   // 오버랩이 발생한 자기 자신
			AActor* OtherActor,					// Sphere에 부딛힌 액터
			UPrimitiveComponent* OtherComp,		// 부딛힌 액터에 붙어있고 충돌을 일으킨 원인인 컴포넌트
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;
	virtual void ActivateItem(AActor* Activator) override;
};
