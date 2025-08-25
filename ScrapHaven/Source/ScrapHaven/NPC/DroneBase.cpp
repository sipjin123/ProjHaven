// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/DroneBase.h"

// Sets default values
ADroneBase::ADroneBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

