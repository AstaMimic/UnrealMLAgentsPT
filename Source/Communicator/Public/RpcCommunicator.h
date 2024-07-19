#pragma once

#include "CoreMinimal.h"
#include "ActionSpec.h"
#include "AgentInfo.h"
#include "grpc++/grpc++.h"
#include "CommunicatorInterface.h"
#include "ObservationWriter.h"
#include "google/protobuf/repeated_field.h"
#include "mlagents_envs/communicator_objects/unreal_to_external.grpc.pb.h"
#include "mlagents_envs/communicator_objects/unreal_rl_output.pb.h"
#include "mlagents_envs/communicator_objects/unreal_message.pb.h"
#include "RpcCommunicator.generated.h"

/**
 * 
 */
UCLASS()
class COMMUNICATOR_API URpcCommunicator: public UObject, public ICommunicatorInterface
{
    GENERATED_BODY()

public:
	URpcCommunicator();

    virtual FQuitCommandHandler& OnQuitCommandReceived() override;
    virtual FResetCommandHandler& OnResetCommandReceived() override;
    virtual FRLInputReceivedHandler& OnRLInputReceived() override; 

    virtual bool Initialize(const FCommunicatorInitParameters& InitParameters, FUnrealRLInitParameters& InitParametersOut) override;
    virtual void SubscribeBrain(const FString& Name, FActionSpec ActionSpec) override;
    virtual void PutObservations(const FString& BrainKey, const FAgentInfo& Info, TArray<TScriptInterface<IISensor>>& Sensors) override;
    virtual void DecideBatch() override;
    virtual const FActionBuffers GetActions(const FString& Key, int32 AgentId) override;

private:	
	bool bIsOpen;
	
	// Unreal to external Client
	std::unique_ptr<communicator_objects::UnrealToExternalProto::Stub> Stub;
	std::shared_ptr<grpc::Channel> Channel;

    UFUNCTION()
	void NotifyQuitAndShutDownChannel();

	communicator_objects::UnrealInputProto Initialize(int32 Port, const communicator_objects::UnrealOutputProto & UnrealOutput, communicator_objects::UnrealInputProto& UnrealInput);
	static communicator_objects::UnrealMessageProto WrapMessage(const communicator_objects::UnrealOutputProto* Content, int32 Status);
	static bool CheckCommunicationVersionAreCompatible(const FString& unityCommunicationVersion, const FString& pythonApiVersion);
    bool EstablishConnection(int32 Port);
    bool SendAndReceiveMessage(const communicator_objects::UnrealMessageProto& Request, communicator_objects::UnrealMessageProto& Response);
    void CacheActionSpec(const FString& Name, FActionSpec ActionSpec);
    void SendBatchedMessageHelper();
    communicator_objects::UnrealInputProto* Exchange(const communicator_objects::UnrealOutputProto* UnrealOutput);
    communicator_objects::UnrealRLInitializationOutputProto* GetTempUnrealRlInitializationOutput();
    void UpdateSentActionSpec(const communicator_objects::UnrealRLInitializationOutputProto& Output);
    void SendCommandEvent(communicator_objects::CommandProto Command);

    // Extension
    communicator_objects::ObservationProto GetObservationProto(TScriptInterface<IISensor> Sensor, ObservationWriter& ObsWriter);
    FActionBuffers ToActionBuffers(const communicator_objects::AgentActionProto& Proto);
    communicator_objects::ActionSpecProto ToActionSpecProto(const FActionSpec& ActionSpec);
    TArray<FActionBuffers> ToAgentActionList(const communicator_objects::UnrealRLInputProto_ListAgentActionProto Proto);
    communicator_objects::BrainParametersProto ToBrainParametersProto(const FActionSpec& ActionSpec, FString Name, bool bIsTraining);
    communicator_objects::AgentInfoProto ToAgentInfoProto(const FAgentInfo& Info);

    FQuitCommandHandler QuitCommandReceived;
    FResetCommandHandler ResetCommandReceived;
    FRLInputReceivedHandler RLInputReceived;

    bool bNeedCommunicateThisStep;

    ObservationWriter ObsWriter;
    // TMap<FString, USensorShapeValidator*> SensorShapeValidators;
    TMap<FString, TArray<int32>> OrderedAgentsRequestingDecisions;
    communicator_objects::UnrealRLOutputProto* CurrentUnrealRlOutput;
    TMap<FString, TMap<int32, FActionBuffers>> LastActionsReceived;
    TSet<FString> SentBrainKeys;
    TMap<FString, FActionSpec> UnsentBrainKeys;
    TArray<FString> BehaviorNames;
};
