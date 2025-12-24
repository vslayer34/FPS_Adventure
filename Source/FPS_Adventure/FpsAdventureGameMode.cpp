// Fill out your copyright notice in the Description page of Project Settings.


#include "FpsAdventureGameMode.h"


void AFpsAdventureGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Cyan, TEXT("C++ is a pain in the ass"));
}

