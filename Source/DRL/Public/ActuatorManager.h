#pragma once

#include "CoreMinimal.h"
#include "IActuator.h"
#include "IActionReceiver.h"
#include "ActuatorDiscreteActionMask.h"
#include "ActuatorManager.generated.h"

UCLASS()
class DRL_API UActuatorManager : public UObject
{
    GENERATED_BODY()

public:
    // Constructor
    UActuatorManager();

    // Initialization with a preset capacity
    void Initialize(int32 Capacity = 0);

    // Getters
    int32 GetSumOfDiscreteBranchSizes() const { return SumOfDiscreteBranchSizes; }
    int32 GetNumDiscreteActions() const { return NumDiscreteActions; }
    int32 GetNumContinuousActions() const { return NumContinuousActions; }
    int32 GetTotalNumberOfActions() const { return NumContinuousActions + NumDiscreteActions; }
    const UActuatorDiscreteActionMask& GetDiscreteActionMask() const;
    FActionBuffers& GetStoredActions() { return StoredActions; }

    // Methods
    void ReadyActuatorsForExecution();
    void UpdateActions(const FActionBuffers& Actions);
    FActionSpec GetCombinedActionSpec();
    void WriteActionMask();
    void ApplyHeuristic(const FActionBuffers& ActionBuffersOut);
    void ExecuteActions();
    void ResetData();

    void Add(const TScriptInterface<IActuator>& Item);
    void Clear();
    bool Contains(const TScriptInterface<IActuator>& Item) const;
    void CopyTo(TArray<TScriptInterface<IActuator>>& OutArray, int32 ArrayIndex) const;
    bool Remove(const TScriptInterface<IActuator>& Item);

    int32 Count() const;
    bool IsReadOnly() const;
    int32 IndexOf(const TScriptInterface<IActuator>& Item) const;
    void Insert(int32 Index, const TScriptInterface<IActuator>& Item);
    void RemoveAt(int32 Index);

    TScriptInterface<IActuator> Get(int32 Index) const;
    void Set(int32 Index, const TScriptInterface<IActuator>& Value);
    TScriptInterface<IActuator> operator[](int32 Index) const;
    TScriptInterface<IActuator>& operator[](int32 Index);
    TArray<TScriptInterface<IActuator>>::TConstIterator CreateConstIterator() const;
    TArray<TScriptInterface<IActuator>>::TIterator CreateIterator();

private:
    // Actuators managed by this object.
    UPROPERTY()
    TArray<TScriptInterface<IActuator>> Actuators;

    // An implementation of IDiscreteActionMask that allows for writing to it based on an offset.
    UPROPERTY()
    UActuatorDiscreteActionMask* DiscreteActionMask;

    // Combined action specification.
    UPROPERTY()
    FActionSpec CombinedActionSpec;

    // The currently stored ActionBuffers object for the IActuator managed by this class.
    UPROPERTY()
    FActionBuffers StoredActions;

    // Flag used to check if our actuators are ready for execution.
    bool bReadyForExecution;

    // The sum of all of the discrete branches for all of the IActuator in this manager.
    int32 SumOfDiscreteBranchSizes;

    // The number of the discrete branches for all of the IActuator in this manager.
    int32 NumDiscreteActions;

    // The number of continuous actions for all of the IActuator in this manager.
    int32 NumContinuousActions;

    // Helper methods
    void ReadyActuatorsForExecution(const TArray<TScriptInterface<IActuator>>& InActuators, int32 InNumContinuousActions, int32 InSumOfDiscreteBranchSizes, int32 InNumDiscreteBranches);
    static FActionSpec CombineActionSpecs(const TArray<TScriptInterface<IActuator>>& Actuators);
    void ValidateActuators();
    void SortActuators(TArray<TScriptInterface<IActuator>>& InActuators);

    template <typename T>
    static void UpdateActionArray(const FActionSegment<T>& SourceActionBuffer, FActionSegment<T>& Destination);

    void AddToBufferSizes(const TScriptInterface<IActuator>& ActuatorItem);
	void SubtractFromBufferSizes(const TScriptInterface<IActuator>& ActuatorItem);
    void ClearBufferSizes();
    void AddActuators(const TArray<TScriptInterface<IActuator>>& InActuators);
};