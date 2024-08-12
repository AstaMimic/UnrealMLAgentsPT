#pragma once

#include "CoreMinimal.h"
#include "IActuator.h"
#include "IBuiltInActuator.h"
#include "IHeuristicProvider.h"
#include "VectorActuator.generated.h"

UCLASS(Blueprintable)
class DRL_API UVectorActuator : public UObject, public IActuator, public IBuiltInActuator
{
    GENERATED_BODY()

public:
    // Constructor
    UVectorActuator();

    // Overloaded constructors
    void Initialize(IActionReceiver* InActionReceiver, const FActionSpec& InActionSpec, const FString& InName = "VectorActuator");
    void Initialize(IActionReceiver* InActionReceiver, IHeuristicProvider* InHeuristicProvider, const FActionSpec& InActionSpec, const FString& InName = "VectorActuator");

    // IActuator Interface
    virtual void ResetData() override;
    virtual void OnActionReceived(const FActionBuffers& ActionBuffers) override;
    virtual void Heuristic(FActionBuffers& ActionBuffersOut) override;
    virtual void WriteDiscreteActionMask(const TScriptInterface<IDiscreteActionMask>& ActionMask) override;

    // IBuiltInActuator Interface
    virtual EBuiltInActuatorType GetBuiltInActuatorType() const override;

    // Getters for ActionSpec and Name
    const FActionSpec GetActionSpec() const { return ActionSpec; }
    const FString GetName() const { return Name; }

private:
    IActionReceiver* ActionReceiver;
    IHeuristicProvider* HeuristicProvider;  
    FActionBuffers ActionBuffers;
    FActionSpec ActionSpec;
    FString Name;
};