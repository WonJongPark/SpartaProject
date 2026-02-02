#include "MineItem.h"

#include "SpartaGameState.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 3.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bHasExploded = false;
	
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	
	Super::ActivateItem(Activator);
	// 게임 월드에는 타이머 매니저 여러개가 있음
	// 타이머 매니저가 타이머 핸들러를 관리
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AMineItem::Explode,
		ExplosionDelay,
		false
	);
	
	bHasExploded = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}
	
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}
	
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass() // 가장 기본적인 데미지 유형
			);
			
			if (UWorld* World = GetWorld())
			{
				if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
				{
					GameState->UpdateHUD();
				}
			}
		}
	}
	
	DestroyItem();
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		
		GetWorldTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]() // 람다 함수.
			{							  // 대괄호 안에 있는 것은 캡처리스트로 Particle이란 변수를
										  // 밖의 스코프에서 가져와서 사용하게 만들 수 있다.
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
				
			},
			2.0f,
			false
		);
	}
}
