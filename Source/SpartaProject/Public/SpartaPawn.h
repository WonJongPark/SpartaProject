#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpartaPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// UFUNCTION()
	// void Move(const FInputActionValue& value);
	//
	// UFUNCTION()
	// void StartJump(const FInputActionValue& value);
	//
	// UFUNCTION()
	// void StopJump(const FInputActionValue& value);
	//
	// UFUNCTION()
	// void Look(const FInputActionValue& value);
	//
	// UFUNCTION()
	// void StartSprint(const FInputActionValue& value);
	//
	// UFUNCTION()
	// void StopSprint(const FInputActionValue& value);

private:

	float NormalSpeed;
};
