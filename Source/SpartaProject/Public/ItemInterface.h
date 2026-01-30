#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
// 리플렉션 시스템을 위한 인터페이스
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SPARTAPROJECT_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 지뢰, 힐링 코인
	// 힐링, 코인 - 즉시 발동형 -> 오버랩
	// 지뢰 - 범위 내에 오버랩 -> 발동이 된 후 5초뒤 폭발 -> 오버랩 -> 데미지
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlapComp,   // 오버랩이 발생한 자기 자신
		AActor* OtherActor,					// Sphere에 부딛힌 액터
		UPrimitiveComponent* OtherComp,		// 부딛힌 액터에 붙어있고 충돌을 일으킨 원인인 컴포넌트
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlapComp,   // 오버랩이 발생한 자기 자신
		AActor* OtherActor,					// Sphere에 부딛힌 액터
		UPrimitiveComponent* OtherComp,		// 부딛힌 액터에 붙어있고 충돌을 일으킨 원인인 컴포넌트
		int32 OtherBodyIndex) = 0;
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual FName GetItemType() const = 0; // type형만 알아낼 경우에는 FString보다 FName이 더 빠르다.
};
