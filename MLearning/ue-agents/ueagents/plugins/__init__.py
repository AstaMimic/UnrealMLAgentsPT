from typing import Dict, Any

UE_AGENTS_STATS_WRITER = "ueagents.stats_writer"
UE_AGENTS_TRAINER_TYPE = "ueagents.trainer_type"

# TODO: the real type is Dict[str, HyperparamSettings]
all_trainer_types: Dict[str, Any] = {}
all_trainer_settings: Dict[str, Any] = {}
