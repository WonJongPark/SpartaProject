#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadWidget;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float Health;
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(
		float DamageAmount, // 데미지 양
		FDamageEvent const& DamageEvent, // 데미지를 받는 유형(스킬에 따라 어떤 데미지인지)
		AController* EventInstigator, // 데미지를 누가 입혔는지(지뢰를 심은 사람)
		AActor* DamageCauser) // 데미지를 일으킨 엑터
	override; 
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	
	void OnDeath();
	void UpdateOverheadHP();
	

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
