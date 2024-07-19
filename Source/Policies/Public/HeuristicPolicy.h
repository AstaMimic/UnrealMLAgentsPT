#pragma once

#include "CoreMinimal.h"
#include "IPolicy.h"
#include "AgentInfo.h"
#include "ISensor.h"
#include "ActuatorManager.h"
#include "ObservationWriter.h"
#include "HeuristicPolicy.generated.h"

UCLASS()
class POLICIES_API UHeuristicPolicy : public UObject, public IIPolicy
{
    GENERATED_BODY()

private:
    UPROPERTY()
    UActuatorManager* ActuatorManager;

    FActionBuffers ActionBuffers;
    bool bDone;
    bool bDecisionRequested;

    ObservationWriter ObservationWriter;
    TArray<FString> NullList;

public:
    UHeuristicPolicy();

    void Initialize(UActuatorManager* InActuatorManager, const FActionSpec& ActionSpec);

    virtual void RequestDecision(const FAgentInfo& Info, TArray<TScriptInterface<IISensor>>& Sensors) override;

    virtual const FActionBuffers& DecideAction() override;
};