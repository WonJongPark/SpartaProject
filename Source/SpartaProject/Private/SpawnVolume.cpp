#include "SpawnVolume.h"

#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
	
	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	// 스케일이 적용된 박스 컴포넌트의 절반 길이만큼 반환
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// (200, 100, 50) = (2, 1, 1) -> (400, 100, 50)
	// 중심부터 끝까지의 거리
	
	// 중심좌표 얻기
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
	FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
	 FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
	FMath::FRandRange(0.f, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get()) // TSoftPtr로 반환 되므로 UClass로 반환이됨
		{
			return SpawnItem(ActualClass);
		}
	}
	
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;
	
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows); // 첫 번째 인자는 가져오지 못한 행을 알려주는 용
	
	if (AllRows.IsEmpty()) return nullptr;
	
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f; // 누적 랜덤값 뽑기 알고리즘
	
	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	
	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;
	
	// 매개변수가 AActor의 자손 클래스만 오기 때문에 Spawn도 AActor로 통일
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass, // 스폰할 것
		GetRandomPointInVolume(), // 스폰 위치
		FRotator::ZeroRotator
	);
	
	return SpawnedActor;
}

