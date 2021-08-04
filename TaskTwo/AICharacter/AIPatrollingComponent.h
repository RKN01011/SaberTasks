// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PatrollingPath.h"

#include "AIPatrollingComponent.generated.h"

class APatrollingPath;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERS_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanPatrol() const;

	FWayPointInfo GetCurrentWayPointInfo() const;
	
	FVector SelectClosestWaypoint();
	
	FVector SelectNextWaypoint();
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Path")
	APatrollingPath* PatrollingPath = nullptr;

private:
	int32 PrevWaypointIndex = -1;
	int32 CurrentWaypointIndex = -1;
};
