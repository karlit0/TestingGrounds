// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the patrol route
	APawn* controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	UPatrolRoute* patrolRoute = controlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(patrolRoute))
	{
		return EBTNodeResult::Failed;
	}

	// Warn about empty patrol routes
	TArray<AActor*> patrolPoints = patrolRoute->GetPatrolPoints();
	if (patrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Guard(%s) is missing patrol points!"), *controlledPawn->GetName());
	}
	else
	{
		// Set next waypoint
		UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
		int32 index = blackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
		blackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, patrolPoints[index]);

		// Cycle the index
		int32 nextIndex = (index + 1) % patrolPoints.Num();
		blackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, nextIndex);
	}

	return EBTNodeResult::Succeeded;
}
