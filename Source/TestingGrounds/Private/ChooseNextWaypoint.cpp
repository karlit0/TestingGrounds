// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingGuard.h" // TODO remove coupling
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against empty patrol routes

	// Get the patrol points
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* controlledPawn = AIController->GetPawn();
	APatrollingGuard* patrollingGuard = Cast<APatrollingGuard>(controlledPawn);
	TArray<AActor*> patrolPoints = patrollingGuard->PatrolPointsCPP;

	// Set next waypoint
	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
	int32 index = blackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	blackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, patrolPoints[index]);

	// Cycle the index
	int32 nextIndex = (index + 1) % patrolPoints.Num();
	blackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, nextIndex);

	return EBTNodeResult::Succeeded;
}
