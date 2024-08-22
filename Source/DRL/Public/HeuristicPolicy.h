#pragma once

#include "CoreMinimal.h"
#include "IPolicy.h"
#include "AgentInfo.h"
#include "ISensor.h"
#include "ActuatorManager.h"
#include "ObservationWriter.h"
#include "HeuristicPolicy.generated.h"

UCLASS()
class DRL_API UHeuristicPolicy : public UObject, public IIPolicy
{
    GENERATED_BODY()

private:
    UPROPERTY()
    UActuatorManager* ActuatorManager;

    UPROPERTY()
    FActionBuffers ActionBuffers;

    UPROPERTY()
    TArray<FString> NullList;

    ObservationWriter ObservationWriter;

    bool bDone;
    bool bDecisionRequested;


public:
    UHeuristicPolicy();

    void Initialize(UActuatorManager* InActuatorManager, const FActionSpec& ActionSpec);

    virtual void RequestDecision(const FAgentInfo& Info, TArray<TScriptInterface<IISensor>>& Sensors) override;

    virtual const FActionBuffers& DecideAction() override;
};