// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrollingComponent.h"

#include "AICharacter/Navigation/PatrollingPath.h"

bool UAIPatrollingComponent::CanPatrol() const
{
	return IsValid(PatrollingPath) && PatrollingPath -> GetWaypointsInfo().Num() > 0;
}

FWayPointInfo UAIPatrollingComponent::GetCurrentWayPointInfo() const
{
	const TArray<FWayPointInfo>& WayPoints = PatrollingPath -> GetWaypointsInfo();
	return WayPoints[FMath::Max(0, PrevWaypointIndex)];
}

FVector UAIPatrollingComponent::SelectClosestWaypoint()
{
	FVector OwnerLocation = GetOwner() -> GetActorLocation();
	const TArray<FWayPointInfo>& WayPoints = PatrollingPath -> GetWaypointsInfo();
	FTransform PathTransform = PatrollingPath -> GetActorTransform();

	FVector ClosestWayPoint = PathTransform.TransformPosition(WayPoints[0].WayPoint);
	float PreviousSqDistance = (OwnerLocation - ClosestWayPoint).SizeSquared();
	CurrentWaypointIndex = 0;
	
	for (int32 i = 1; i < WayPoints.Num(); i++)
	{
		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i].WayPoint);
		const float CurrentSqDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		
		if (CurrentSqDistance < PreviousSqDistance)
		{
			ClosestWayPoint = WayPointWorld;
			PreviousSqDistance = CurrentSqDistance;
			CurrentWaypointIndex = i;
		}
	}

	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::SelectNextWaypoint()
{
	PrevWaypointIndex = CurrentWaypointIndex;
	++CurrentWaypointIndex;
	
	const TArray<FWayPointInfo>& WayPoints = PatrollingPath -> GetWaypointsInfo();
	if (CurrentWaypointIndex == PatrollingPath -> GetWaypointsInfo().Num())
	{
		CurrentWaypointIndex = 0;
	}

	FTransform PathTransform = PatrollingPath -> GetActorTransform();
	return PathTransform.TransformPosition(WayPoints[CurrentWaypointIndex].WayPoint);
}
