// Fill out your copyright notice in the Description page of Project Settings.


#include "RpcCommunicator.h"

URpcCommunicator::URpcCommunicator()
	: bIsOpen(false)
{
}

bool URpcCommunicator::Initialize(const FCommunicatorInitParameters& InitParameters, FUnrealRLInitParameters& InitParametersOut)
{
    communicator_objects::UnrealRLInitializationOutputProto AcademyParameters;
    AcademyParameters.set_name(TCHAR_TO_UTF8(*InitParameters.Name));
    AcademyParameters.set_package_version(TCHAR_TO_UTF8(*InitParameters.UnrealPackageVersion));
    AcademyParameters.set_communication_version(TCHAR_TO_UTF8(*InitParameters.UnrealCommunicationVersion));

    communicator_objects::UnrealInputProto Input;
    communicator_objects::UnrealInputProto InitializationInput;

    try
    {
        // Create UnrealOutputProto and set the RLInitializationOutput
        communicator_objects::UnrealOutputProto UnrealOutput;
        *UnrealOutput.mutable_rl_initialization_output() = AcademyParameters;

        InitializationInput = Initialize(
            InitParameters.Port,
            UnrealOutput,
            Input
        );
    }
    catch (const std::exception& ex)
    {
        UE_LOG(LogTemp, Error, TEXT("Unexpected exception when trying to initialize communication: %s"), UTF8_TO_TCHAR(ex.what()));
        InitParametersOut = FUnrealRLInitParameters();
        NotifyQuitAndShutDownChannel();
        return false;
    }

    FString PythonPackageVersion = UTF8_TO_TCHAR(InitializationInput.rl_initialization_input().package_version().c_str());
    FString PythonCommunicationVersion = UTF8_TO_TCHAR(InitializationInput.rl_initialization_input().communication_version().c_str());

    bool CommunicationIsCompatible = CheckCommunicationVersionAreCompatible(
        InitParameters.UnrealCommunicationVersion,
        PythonCommunicationVersion
    );

    // Initialization succeeded part-way. The most likely cause is a mismatch between the communicator API strings.
    if (InitializationInput.has_rl_initialization_input() && !Input.has_rl_input())
    {
        if (!CommunicationIsCompatible)
        {
            UE_LOG(LogTemp, Warning, TEXT("Communication protocol between python (%s) and Unreal (%s) have different versions which make them incompatible. Python library version: %s."),
                   *PythonCommunicationVersion, *InitParameters.UnrealCommunicationVersion, *PythonPackageVersion);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Unknown communication error between Python. Python communication protocol: %s, Python library version: %s."),
                   *PythonCommunicationVersion, *PythonPackageVersion);
        }

        InitParametersOut = FUnrealRLInitParameters();
        return false;
    }

    // UpdateEnvironmentWithInput(Input.rlinitializationinput()); // Implement this if needed
    InitParametersOut.Seed = InitializationInput.rl_initialization_input().seed();
    InitParametersOut.NumAreas = InitializationInput.rl_initialization_input().num_areas();
    InitParametersOut.PythonLibraryVersion = UTF8_TO_TCHAR(InitializationInput.rl_initialization_input().package_version().c_str());
    InitParametersOut.PythonCommunicationVersion = UTF8_TO_TCHAR(InitializationInput.rl_initialization_input().communication_version().c_str());

    // Be sure to shut down the grpc channel when the application is quitting.
    FCoreDelegates::OnPreExit.AddUObject(this, &URpcCommunicator::NotifyQuitAndShutDownChannel);

    return true;
}

bool URpcCommunicator::EstablishConnection(int32 Port)
{
    std::string ServerAddress = "localhost:" + std::to_string(Port);
    Channel = grpc::CreateChannel(ServerAddress, grpc::InsecureChannelCredentials());
    Stub = communicator_objects::UnrealToExternalProto::NewStub(Channel);
    return Channel != nullptr && Stub != nullptr;
}

bool URpcCommunicator::SendAndReceiveMessage(const communicator_objects::UnrealMessageProto & Request, communicator_objects::UnrealMessageProto& Response)
{
    grpc::ClientContext Context;
    grpc::Status Status = Stub->Exchange(&Context, Request, &Response);

    if (!Status.ok())
    {
        bIsOpen = false;
        NotifyQuitAndShutDownChannel();
        throw std::runtime_error(Status.error_message());
    }

    return Response.header().status() == 200;
}

communicator_objects::UnrealInputProto URpcCommunicator::Initialize(int32 Port, const communicator_objects::UnrealOutputProto& UnrealOutput, communicator_objects::UnrealInputProto& UnrealInput)
{
    bIsOpen = true;

    if (!EstablishConnection(Port))
    {
        throw std::runtime_error("Failed to establish connection");
    }

    communicator_objects::UnrealMessageProto Result;
    communicator_objects::UnrealMessageProto InputMessage;

    if (!SendAndReceiveMessage(WrapMessage(&UnrealOutput, 200), Result))
    {
        throw std::runtime_error("Failed to receive proper response");
    }

    if (!SendAndReceiveMessage(WrapMessage(nullptr, 200), InputMessage))
    {
        throw std::runtime_error("Failed to receive input message");
    }

    UnrealInput = InputMessage.unreal_input();

    if (Result.header().status() != 200 || InputMessage.header().status() != 200)
    {
        bIsOpen = false;
        NotifyQuitAndShutDownChannel();
    }

    return Result.unreal_input();
}


communicator_objects::UnrealMessageProto URpcCommunicator::WrapMessage(const communicator_objects::UnrealOutputProto* Content, int32 Status)
{
    communicator_objects::UnrealMessageProto Message;
    auto* Header = Message.mutable_header();
    Header->set_status(Status);
    if (Content != nullptr)
    {
		*Message.mutable_unreal_output() = *Content;
    }
    return Message;
}

bool URpcCommunicator::CheckCommunicationVersionAreCompatible(const FString& UnrealCommunicationVersion, const FString& PythonApiVersion)
{
    TArray<FString> UnrealParts;
    UnrealCommunicationVersion.ParseIntoArray(UnrealParts, TEXT("."));
    int32 UnrealMajor = FCString::Atoi(*UnrealParts[0]);
    int32 UnrealMinor = UnrealParts.Num() > 1 ? FCString::Atoi(*UnrealParts[1]) : 0;

    TArray<FString> PythonParts;
    PythonApiVersion.ParseIntoArray(PythonParts, TEXT("."));
    int32 PythonMajor = FCString::Atoi(*PythonParts[0]);
    int32 PythonMinor = PythonParts.Num() > 1 ? FCString::Atoi(*PythonParts[1]) : 0;

    if (UnrealMajor == 0)
    {
        if (UnrealMajor != PythonMajor || UnrealMinor != PythonMinor)
        {
            return false;
        }
    }
    else if (UnrealMajor != PythonMajor)
    {
        return false;
    }
    else if (UnrealMinor != PythonMinor)
    {
        // If a feature is used in Unreal but not supported in the trainer,
        // we will warn at the point it's used. Don't warn here to avoid noise.
    }

    return true;
}

void URpcCommunicator::NotifyQuitAndShutDownChannel()
{
	Channel.reset();
	Stub.reset();
	QuitCommandReceived.Broadcast();
}

FQuitCommandHandler& URpcCommunicator::OnQuitCommandReceived()
{
    return QuitCommandReceived;
}

FResetCommandHandler& URpcCommunicator::OnResetCommandReceived()
{
    return ResetCommandReceived;
}

FRLInputReceivedHandler& URpcCommunicator::OnRLInputReceived()
{
    return RLInputReceived;
}

void URpcCommunicator::SubscribeBrain(const FString& Name, const FString& ActionSpec)
{
    // Implement the logic for subscribing a brain here
}

void URpcCommunicator::PutObservations(const FString& BrainKey, const FString& AgentInfo, const TArray<FString>& Sensors)
{
    // Implement the logic for putting observations here
}

void URpcCommunicator::DecideBatch()
{
    // Implement the logic for deciding a batch here
}

FString URpcCommunicator::GetActions(const FString& Key, int32 AgentId)
{
    // Implement the logic for getting actions here
    return FString();
}
