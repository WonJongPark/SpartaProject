// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPawn.h"

// Sets default values
ASpartaPawn::ASpartaPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpartaPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpartaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

