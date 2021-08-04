// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrollingPath.generated.h"

USTRUCT(BlueprintType)
struct FWayPointInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector WayPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool ItIsSleepPoint = false;
};

UCLASS()
class CHARACTERS_API APatrollingPath : public AActor
{
	GENERATED_BODY()
	
public:
	const TArray<FWayPointInfo>& GetWaypointsInfo() const;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Path")
	TArray<FWayPointInfo> WayPoints;
};
