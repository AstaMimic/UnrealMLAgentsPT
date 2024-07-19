#include "RemotePolicy.h"
#include "DRLAcademy.h"

URemotePolicy::URemotePolicy()
    : AgentId(0), Communicator(nullptr)
{
}

void URemotePolicy::Initialize(UActuatorManager* InActuatorManager, const FActionSpec& InActionSpec, const FString& InFullyQualifiedBehaviorName)
{
    Communicator = UDRLAcademy::GetInstance()->RpcCommunicator;
    if (Communicator)
    {
       Communicator->SubscribeBrain(FullyQualifiedBehaviorName, ActionSpec);
    }
    ActionSpec = InActionSpec;
}

void URemotePolicy::RequestDecision(const FAgentInfo& Info, TArray<TScriptInterface<IISensor>>& Sensors)
{
    AgentId = Info.EpisodeId;
    if (Communicator)
    {
        Communicator->PutObservations(FullyQualifiedBehaviorName, Info, Sensors);
    }
}

const FActionBuffers& URemotePolicy::DecideAction()
{
    if (Communicator)
    {
        Communicator->DecideBatch();
        const FActionBuffers Actions = Communicator->GetActions(FullyQualifiedBehaviorName, AgentId);
        LastActionBuffer = Actions.IsEmpty() ? Actions : FActionBuffers::Empty;
    }
    return LastActionBuffer;
}
