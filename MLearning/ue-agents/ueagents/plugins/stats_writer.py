import importlib.metadata as importlib_metadata  # pylint: disable=E0611
from typing import List

from ueagents.plugins import UE_AGENTS_STATS_WRITER
from ueagents.trainers.settings import RunOptions
from ueagents.trainers.stats import StatsWriter
from ueagents.trainers.stats import TensorboardWriter, GaugeWriter, ConsoleWriter
from ueagents_envs import logging_util

logger = logging_util.get_logger(__name__)


def get_default_stats_writers(run_options: RunOptions) -> List[StatsWriter]:
    """
    The StatsWriters that mlagents-learn always uses:
    * A TensorboardWriter to write information to TensorBoard
    * A GaugeWriter to record our internal stats
    * A ConsoleWriter to output to stdout.
    """
    checkpoint_settings = run_options.checkpoint_settings
    return [
        TensorboardWriter(
            checkpoint_settings.write_path,
            clear_past_data=not checkpoint_settings.resume,
            hidden_keys=["Is Training", "Step"],
        ),
        GaugeWriter(),
        ConsoleWriter(),
    ]


def register_stats_writer_plugins(run_options: RunOptions) -> List[StatsWriter]:
    """
    Registers all StatsWriter plugins (including the default one),
    and evaluates them, and returns the list of all the StatsWriter implementations.
    """
    all_stats_writers: List[StatsWriter] = []
    if UE_AGENTS_STATS_WRITER not in importlib_metadata.entry_points():
        logger.warning(
            f"Unable to find any entry points for {UE_AGENTS_STATS_WRITER}, even the default ones. "
            "Uninstalling and reinstalling ue-agents via pip should resolve. "
            "Using default plugins for now."
        )
        return get_default_stats_writers(run_options)

    entry_points = importlib_metadata.entry_points()[UE_AGENTS_STATS_WRITER]

    for entry_point in entry_points:

        try:
            logger.debug(f"Initializing StatsWriter plugins: {entry_point.name}")
            plugin_func = entry_point.load()
            plugin_stats_writers = plugin_func(run_options)
            logger.debug(
                f"Found {len(plugin_stats_writers)} StatsWriters for plugin {entry_point.name}"
            )
            all_stats_writers += plugin_stats_writers
        except BaseException:
            # Catch all exceptions from setting up the plugin, so that bad user code doesn't break things.
            logger.exception(
                f"Error initializing StatsWriter plugins for {entry_point.name}. This plugin will not be used."
            )
    return all_stats_writers
