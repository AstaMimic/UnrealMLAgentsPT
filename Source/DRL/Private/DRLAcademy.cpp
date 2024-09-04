// Fill out your copyright notice in the Description page of Project Settings.


#include "DRLAcademy.h"
#include "GenericPlatform/GenericPlatformMisc.h"

UDRLAcademy* UDRLAcademy::Instance = nullptr;

UDRLAcademy::UDRLAcademy() {
    FCoreDelegates::OnExit.AddUObject(this, &UDRLAcademy::Dispose);
    StepRecursionChecker->Initialize("EnvironmentStep");
}

// Make the Academy Tick
bool UDRLAcademy::IsTickable() const {
    return true;
}

void UDRLAcademy::Tick(float DeltaTime) {
    EnvironmentStep();
}

TStatId UDRLAcademy::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ThisClassName, STATGROUP_Tickables);
}

UDRLAcademy* UDRLAcademy::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UDRLAcademy>();
        Instance->AddToRoot(); 
        Instance->LazyInitialize();
    }
    return Instance;
}

void UDRLAcademy::LazyInitialize() {
    if (!bInitialized) {
        InitializeEnvironment();
        bInitialized = true;
    }
}

void UDRLAcademy::ParseCommandLineArgs() {

    FString PortString;
    if (FParse::Value(FCommandLine::Get(), TEXT("mlAgentPort="), PortString))
    {
        Port = FCString::Atoi(*PortString);
        UE_LOG(LogTemp, Log, TEXT("Using custom mlAgentPort: %d"), Port);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Using default mlAgentPort: %d"), Port);
    }
}

void UDRLAcademy::InitializeEnvironment() {
    UE_LOG(LogTemp, Log, TEXT("Initialize Environement"));

    bEnableStepping = true;
    ParseCommandLineArgs();

	RpcCommunicator = NewObject<URpcCommunicator>();

    if (RpcCommunicator != nullptr) {
        bool bInitSuccessful = false;
        FCommunicatorInitParameters CommunicatorInitParams;
        CommunicatorInitParams.Port = Port;
        CommunicatorInitParams.UnrealCommunicationVersion = ApiVersion;
        CommunicatorInitParams.UnrealPackageVersion = PackageVersion;
        CommunicatorInitParams.Name = TEXT("AcademySingleton");

        try
        {
            FUnrealRLInitParameters UnrealRLInitParameters;
            bInitSuccessful = RpcCommunicator->Initialize(CommunicatorInitParams, UnrealRLInitParameters);

            if (bInitSuccessful)
            {
                FMath::SRandInit(UnrealRLInitParameters.Seed);
                InferenceSeed = UnrealRLInitParameters.Seed;
                NumAreas = UnrealRLInitParameters.NumAreas;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Couldn't connect to trainer on port %d using API version %s. Will perform inference instead."), Port, *CommunicatorInitParams.UnrealCommunicationVersion);
                RpcCommunicator = nullptr;
            }
        }
        catch (const std::exception& Ex)
        {
            UE_LOG(LogTemp, Error, TEXT("Unexpected exception when trying to initialize communication: %s\nWill perform inference instead."), UTF8_TO_TCHAR(Ex.what()));
            RpcCommunicator = nullptr;
        }     
    }

    if (RpcCommunicator != nullptr) {
        RpcCommunicator->OnQuitCommandReceived().AddDynamic(this, &UDRLAcademy::OnQuitCommandReceived);
        RpcCommunicator->OnResetCommandReceived().AddDynamic(this, &UDRLAcademy::OnResetCommand);
    }

	// If a communicator is enabled/provided, then we assume we are in
	// training mode. In the absence of a communicator, we assume we are
	// in inference mode.

	ResetActions();
}

void UDRLAcademy::EnvironmentStep() {

    // TODO: Recursion check

    if (!bHadFirstReset)
    {
        ForcedFullReset();
    }

    if (OnAgentPreStep.IsBound())
    {
        OnAgentPreStep.Broadcast(StepCount);
    }

    StepCount += 1;
    TotalStepCount += 1;

    if (OnAgentIncrementStep.IsBound())
    {
        OnAgentIncrementStep.Broadcast();
    }

	if (OnAgentSendState.IsBound())
	{
		OnAgentSendState.Broadcast();
	}

	if (OnDecideAction.IsBound())
	{
		OnDecideAction.Broadcast();
	}

	if (OnAgentAct.IsBound())
	{
		OnAgentAct.Broadcast();
	}

}

void UDRLAcademy::OnQuitCommandReceived() {
    FGenericPlatformMisc::RequestExit(false);
}

void UDRLAcademy::OnResetCommand() {
    ForcedFullReset();
}

void UDRLAcademy::Dispose() {
    OnDecideAction.Clear();
    OnDestroyAction.Clear();
    OnAgentPreStep.Clear();
    OnAgentSendState.Clear();
    OnAgentAct.Clear();
    OnAgentForceReset.Clear();
    OnEnvironmentReset.Clear();
}

void UDRLAcademy::ForcedFullReset() {
    EnvironmentReset();
    if (OnAgentForceReset.IsBound()) {
        OnAgentForceReset.Broadcast();
    }
    bHadFirstReset = true;
}

void UDRLAcademy::EnvironmentReset() {
    StepCount = 0;
    EpisodeCount++;
    if (OnEnvironmentReset.IsBound()) {
        OnEnvironmentReset.Broadcast();
    }
}

void UDRLAcademy::ResetActions() {
    
}

bool UDRLAcademy::IsCommunicatorOn() {
    return RpcCommunicator != nullptr;
}
