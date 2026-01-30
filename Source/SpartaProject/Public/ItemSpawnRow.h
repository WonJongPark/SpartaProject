#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h" // 따로 추가해줘야 함

USTRUCT(BlueprintType) // 블루프린트에서 구조체를 변수로 사용할 수 있게 해줌
struct FItemSpawnRow : public FTableRowBase // 상속 반드시 필수
{
	GENERATED_BODY() // 따로 추가해줘야 함
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
	// TSubclassOf - 하드 레퍼런스
	// 클래스가 항상 메모리에 로드된 상태에서 바로 접근
	// 당장쓸 것이니 메모리에 올려놔라
	// TSoftClassPtr - 소프트 레퍼런스
	// 클래스의 경로만 유지.
	// 필요한 상황이 되면 메모리에 로드
	// 클래스가 100개 등 많아지면 메모리에 올려 놓는건 비효율 적이니
	// 그런 상황에서 사용하고 권장함
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance = 0;
};
