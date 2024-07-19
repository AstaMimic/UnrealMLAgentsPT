#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AgentInfo.h"
#include "ISensor.h"
#include "IPolicy.generated.h"

UINTERFACE(MinimalAPI)
class UIPolicy : public UInterface
{
    GENERATED_BODY()
};

class POLICIES_API IIPolicy
{
    GENERATED_BODY()

public:

    // Signals the Brain that the Agent needs a Decision. The Policy
    // will make the decision at a later time to allow possible
    // batching of requests.
    virtual void RequestDecision(const FAgentInfo& Info, TArray<TScriptInterface<IISensor>>& Sensors) = 0;

    // Signals the Policy that if the Decision has not been taken yet,
    // it must be taken now. The Brain is expected to update the actions
    // of the Agents at this point the latest.
    virtual const FActionBuffers& DecideAction() = 0;
};