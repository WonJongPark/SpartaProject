// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABaseItem();
	
protected:
	virtual void OnItemOverlap(
			UPrimitiveComponent* OverlapComp,   // 오버랩이 발생한 자기 자신
			AActor* OtherActor,					// Sphere에 부딛힌 액터
			UPrimitiveComponent* OtherComp,		// 부딛힌 액터에 붙어있고 충돌을 일으킨 원인인 컴포넌트
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;
	
	virtual void OnItemEndOverlap(
	UPrimitiveComponent* OverlapComp,   // 오버랩이 발생한 자기 자신
			AActor* OtherActor,					// Sphere에 부딛힌 액터
			UPrimitiveComponent* OtherComp,		// 부딛힌 액터에 붙어있고 충돌을 일으킨 원인인 컴포넌트
			int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;
	
	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	TObjectPtr<USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	// 파티클 및 사운드
	// 나이아가라, 캐스케이드 중 캐스케이드에 대해 다룬다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<UParticleSystem> PickupParticle;
	// 언리얼 엔진의 모든 사운드를 공유하는 클래스 베이스
	// 사운드 웨이브와 사운드 큐도 SoundBase를 상속받는다.
	// SoundWave: 간단한 사운드 파일. UI등 짧은 효과음에 사용.
	// SoundCue: 동적 사운드 생성. 여러개의 사운드를 조화 하거나 페이드 블렌딩 등 동적처리가 가능하다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<USoundBase> PickupSound;
};
