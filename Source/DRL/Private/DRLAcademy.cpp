// Fill out your copyright notice in the Description page of Project Settings.


#include "DRLAcademy.h"

UDRLAcademy* UDRLAcademy::Instance = nullptr;

UDRLAcademy::UDRLAcademy() {
    FCoreDelegates::OnExit.AddUObject(this, &UDRLAcademy::Dispose);
}

UDRLAcademy* UDRLAcademy::GetInstance()
{
    if (!Instance)
    {
        Instance = NewObject<UDRLAcademy>();
        Instance->AddToRoot(); 
        Instance->Initialize();
    }
    return Instance;
}

void UDRLAcademy::Initialize() {
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
}

void UDRLAcademy::OnQuitCommandReceived() {

}

void UDRLAcademy::OnResetCommand() {

}

void UDRLAcademy::Dispose() {

}
