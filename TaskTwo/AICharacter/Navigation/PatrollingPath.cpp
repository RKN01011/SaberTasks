// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrollingPath.h"

const TArray<FWayPointInfo>& APatrollingPath::GetWaypointsInfo() const
{
	return WayPoints;
}
