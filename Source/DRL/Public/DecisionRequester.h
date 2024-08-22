#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Agent.h"
#include "DecisionRequester.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DefaultToInstanced)
class DRL_API UDecisionRequester : public UActorComponent
{
    GENERATED_BODY()

public:
    UDecisionRequester();

protected:
    virtual void BeginPlay() override;

public:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DecisionRequester", meta=(ClampMin="1", ClampMax="20"))
    int DecisionPeriod = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DecisionRequester", meta=(ClampMin="0", ClampMax="19"))
    int DecisionStep = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DecisionRequester")
    bool bTakeActionsBetweenDecisions = true;

private:
    UPROPERTY()
    UAgent* Agent;

    void MakeRequests(int AcademyStepCount);

    UFUNCTION()
    void OnAcademyPreStep(int AcademyStepCount);

    bool ShouldRequestDecision(int AcademyStepCount) const;
    bool ShouldRequestAction() const;
};