// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "grpc++/grpc++.h"
#include "CommunicatorInterface.h"
#include "mlagents_envs/communicator_objects/unreal_to_external.grpc.pb.h"
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
    virtual void SubscribeBrain(const FString& Name, const FString& ActionSpec) override;
    virtual void PutObservations(const FString& BrainKey, const FString& AgentInfo, const TArray<FString>& Sensors) override;
    virtual void DecideBatch() override;
    virtual FString GetActions(const FString& Key, int32 AgentId) override;

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

    FQuitCommandHandler QuitCommandReceived;
    FResetCommandHandler ResetCommandReceived;
    FRLInputReceivedHandler RLInputReceived;
};
