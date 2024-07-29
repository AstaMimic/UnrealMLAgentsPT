#include "VectorActuator.h"

UVectorActuator::UVectorActuator()
    : ActionReceiver(nullptr), HeuristicProvider(nullptr)
{
}

void UVectorActuator::Initialize(IActionReceiver* InActionReceiver, const FActionSpec& InActionSpec, const FString& InName)
{
    Initialize(InActionReceiver, Cast<IHeuristicProvider>(InActionReceiver), InActionSpec, InName);
}

void UVectorActuator::Initialize(IActionReceiver* InActionReceiver, IHeuristicProvider* InHeuristicProvider, const FActionSpec& InActionSpec, const FString& InName)
{
    ActionReceiver = InActionReceiver;
    HeuristicProvider = InHeuristicProvider;
    ActionSpec = InActionSpec;

    FString Suffix;
    if (ActionSpec.NumContinuousActions == 0)
    {
        Suffix = "-Discrete";
    }
    else if (ActionSpec.GetNumDiscreteActions() == 0)
    {
        Suffix = "-Continuous";
    }
    else
    {
        Suffix = FString::Printf(TEXT("-Continuous-%d-Discrete-%d"), ActionSpec.NumContinuousActions, ActionSpec.GetNumDiscreteActions());
    }
    Name = InName + Suffix;
}

void UVectorActuator::ResetData()
{
    ActionBuffers = FActionBuffers::Empty;
}

void UVectorActuator::OnActionReceived(const FActionBuffers& InActionBuffers)
{
    ActionBuffers = InActionBuffers;
    if (ActionReceiver)
    {
        ActionReceiver->OnActionReceived(ActionBuffers);
    }
}

void UVectorActuator::Heuristic(const FActionBuffers& ActionBuffersOut)
{
    if (HeuristicProvider)
    {
        HeuristicProvider->Heuristic(ActionBuffersOut);
    }
}

void UVectorActuator::WriteDiscreteActionMask(const TScriptInterface<IDiscreteActionMask>& ActionMask)
{
    if (ActionReceiver)
    {
        ActionReceiver->WriteDiscreteActionMask(ActionMask);
    }
}

EBuiltInActuatorType UVectorActuator::GetBuiltInActuatorType() const
{
    return EBuiltInActuatorType::VectorActuator;
}