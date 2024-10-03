# Unity ML-Agents Protobuf Definitions

Contains relevant definitions needed to generate probobuf files used in [Unreal DRL Toolkit](https://github.com/AlanLaboratory/DeepReinforcementLearning).

## Set-up & Installation

First we will follow these steps once install protobuf and grpcio-tools via your terminal.
Assume the DRL repository is checked out to a folder named $DRL_ROOT.

`pip install -r Protobuf/requirements.txt`

#### On Windows

Download and install the latest version of [nuget](https://www.nuget.org/downloads).

#### On Mac

`brew install nuget`

#### On Linux

`sudo apt-get install nuget`

Navigate to your installation of nuget and run the following:

`nuget install Grpc.Tools -Version 1.14.1 -OutputDirectory $DRL_ROOT/rotobuf-definitions`

## Running

Whenever you change the fields of a message, you must follow the steps below to create C++ and Python files corresponding to the new message.

1. Open a terminal. **Note:** If you're using Anaconda, don't forget to activate the ml-agents environment first.
2. Un-comment line 7 in `make.sh`, and set to correct Grpc.Tools sub-directory.
3. Run the protobuf generation script from the terminal by navigating to `$DRL_ROOT/protobuf-definitions` and entering `make.sh` (for Windows, use `make_for_win.bat`)
4. Note any errors generated that may result from setting the wrong directory in step 2.
