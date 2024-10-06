import logging
from gymnasium.spaces import Box, MultiDiscrete, Tuple as TupleSpace
import numpy as np
import random
import time

from typing import Callable, Optional, Tuple

from ray.rllib.env.multi_agent_env import MultiAgentEnv
from ray.rllib.utils.annotations import PublicAPI
from ray.rllib.utils.typing import AgentID, MultiAgentDict, PolicyID
from ray.rllib.policy.policy import PolicySpec

from ueagents_envs.logging_util import get_logger

logger = get_logger(__name__)


@PublicAPI
class Unreal3DEnv(MultiAgentEnv):
    """A MultiAgentEnv representing a single Unreal3D game instance.

    For an example on how to use this Env with a running Unreal3D editor
    or with a compiled game, see:
    `rllib/examples/unreal3d_env_local.py`
    For an example on how to use it inside a Unreal game client, which
    connects to an RLlib Policy server, see:
    `rllib/examples/envs/external_envs/unreal3d_[client|server].py`

    Supports all Unreal3D (MLAgents) examples, multi- or single-agent and
    gets converted automatically into an ExternalMultiAgentEnv, when used
    inside an RLlib PolicyClient for cloud/distributed training of Unreal games.
    """

    # Default base port when connecting directly to the Editor
    _BASE_PORT_EDITOR = 5004
    # Default base port when connecting to a compiled environment
    _BASE_PORT_ENVIRONMENT = 5005
    # The worker_id for each environment instance
    _WORKER_ID = 0

    def __init__(
        self,
        file_name: Optional[str] = None,
        port: Optional[int] = None,
        seed: int = 0,
        no_graphics: bool = False,
        timeout_wait: int = 300,
        episode_horizon: int = 1000,
    ):
        """Initializes a Unreal3DEnv object.

        Args:
            file_name (Optional[str]): Name of the Unreal game binary.
                If None, will assume a locally running Unreal3D editor
                to be used, instead.
            port (Optional[int]): Port number to connect to Unreal environment.
            seed: A random seed value to use for the Unreal3D game.
            no_graphics: Whether to run the Unreal3D simulator in
                no-graphics mode. Default: False.
            timeout_wait: Time (in seconds) to wait for connection from
                the Unreal3D instance.
            episode_horizon: A hard horizon to abide to. After at most
                this many steps (per-agent episode `step()` calls), the
                Unreal3D game is reset and will start again (finishing the
                multi-agent episode that the game represents).
                Note: The game itself may contain its own episode length
                limits, which are always obeyed (on top of this value here).
        """
        super().__init__()

        if file_name is None:
            print(
                "No game binary provided, will use a running Unreal editor "
                "instead.\nMake sure you are pressing the Play (|>) button in "
                "your editor to start."
            )

        from ueagents_envs.environment import UnrealEnvironment
        from ueagents_envs.exception import UnrealWorkerInUseException

        # Try connecting to the Unreal3D game instance. If a port is blocked
        port_ = None
        while True:
            # Sleep for random time to allow for concurrent startup of many
            # environments (num_env_runners >> 1). Otherwise, would lead to port
            # conflicts sometimes.
            if port_ is not None:
                time.sleep(random.randint(1, 10))
            port_ = port or (
                self._BASE_PORT_ENVIRONMENT if file_name else self._BASE_PORT_EDITOR
            )
            # cache the worker_id and
            # increase it for the next environment
            worker_id_ = Unreal3DEnv._WORKER_ID if file_name else 0
            Unreal3DEnv._WORKER_ID += 1
            try:
                self.unreal_env = UnrealEnvironment(
                    file_name=file_name,
                    worker_id=worker_id_,
                    base_port=port_,
                    seed=seed,
                    no_graphics=no_graphics,
                    timeout_wait=timeout_wait,
                )
                print(
                    "Created UnrealEnvironment for port {}".format(port_ + worker_id_)
                )
            except UnrealWorkerInUseException:
                pass
            else:
                break

        # Reset entire env every this number of step calls.
        self.episode_horizon = episode_horizon
        # Keep track of how many times we have called `step` so far.
        self.episode_timesteps = 0

    def step(
        self, action_dict: MultiAgentDict
    ) -> Tuple[
        MultiAgentDict, MultiAgentDict, MultiAgentDict, MultiAgentDict, MultiAgentDict
    ]:
        """Performs one multi-agent step through the game.

        Args:
            action_dict: Multi-agent action dict with:
                keys=agent identifier consisting of
                [MLagents behavior name, e.g. "Goalie?team=1"] + "_" +
                [Agent index, a unique MLAgent-assigned index per single agent]

        Returns:
            tuple:
                - obs: Multi-agent observation dict.
                    Only those observations for which to get new actions are
                    returned.
                - rewards: Rewards dict matching `obs`.
                - dones: Done dict with only an __all__ multi-agent entry in
                    it. __all__=True, if episode is done for all agents.
                - infos: An (empty) info dict.
        """
        from ueagents_envs.base_env import ActionTuple

        # Set only the required actions (from the DecisionSteps) in Unreal3D.
        all_agents = []
        for behavior_name in self.unreal_env.behavior_specs:
            actions = []
            for agent_id in self.unreal_env.get_steps(behavior_name)[0].agent_id:
                key = behavior_name + "_{}".format(agent_id)
                all_agents.append(key)
                actions.append(action_dict[key])
            if actions:
                if actions[0].dtype == np.float32:
                    action_tuple = ActionTuple(continuous=np.array(actions))
                else:
                    action_tuple = ActionTuple(discrete=np.array(actions))
                self.unreal_env.set_actions(behavior_name, action_tuple)

        # Do the step.
        self.unreal_env.step()

        obs, rewards, terminateds, truncateds, infos = self._get_step_results()
        logger.debug(f"Obs: {obs} - Rewards: {rewards} - Done {terminateds}")

        # Global horizon reached? -> Return __all__ truncated=True, so user
        # can reset. Set all agents' individual `truncated` to True as well.
        self.episode_timesteps += 1
        if self.episode_timesteps > self.episode_horizon:
            return (
                obs,
                rewards,
                terminateds,
                dict({"__all__": True}, **{agent_id: True for agent_id in all_agents}),
                infos,
            )

        return obs, rewards, terminateds, truncateds, infos

    def reset(
        self, *, seed=None, options=None
    ) -> Tuple[MultiAgentDict, MultiAgentDict]:
        """Resets the entire Unreal3D scene (a single multi-agent episode)."""
        self.episode_timesteps = 0
        self.unreal_env.reset()
        obs, _, _, _, infos = self._get_step_results()
        return obs, infos

    def _get_step_results(self):
        """Collects those agents' obs/rewards that have to act in next `step`.

        Returns:
            Tuple:
                obs: Multi-agent observation dict.
                    Only those observations for which to get new actions are
                    returned.
                rewards: Rewards dict matching `obs`.
                dones: Done dict with only an __all__ multi-agent entry in it.
                    __all__=True, if episode is done for all agents.
                infos: An (empty) info dict.
        """
        obs = {}
        rewards = {}
        infos = {}
        for behavior_name in self.unreal_env.behavior_specs:
            decision_steps, terminal_steps = self.unreal_env.get_steps(behavior_name)
            # Important: Only update those sub-envs that are currently
            # available within _env_state.
            # Loop through all envs ("agents") and fill in, whatever
            # information we have.
            for agent_id, idx in decision_steps.agent_id_to_index.items():
                key = behavior_name + "_{}".format(agent_id)
                os = tuple(o[idx] for o in decision_steps.obs)
                os = os[0] if len(os) == 1 else os
                obs[key] = os
                rewards[key] = (
                    decision_steps.reward[idx] + decision_steps.group_reward[idx]
                )
            for agent_id, idx in terminal_steps.agent_id_to_index.items():
                key = behavior_name + "_{}".format(agent_id)
                # Only overwrite rewards (last reward in episode), b/c obs
                # here is the last obs (which doesn't matter anyways).
                # Unless key does not exist in obs.
                if key not in obs:
                    os = tuple(o[idx] for o in terminal_steps.obs)
                    obs[key] = os = os[0] if len(os) == 1 else os
                rewards[key] = (
                    terminal_steps.reward[idx] + terminal_steps.group_reward[idx]
                )

        # Only use dones if all agents are done, then we should do a reset.
        return obs, rewards, {"__all__": False}, {"__all__": False}, infos

    @staticmethod
    def get_policy_configs_for_game(
        game_name: str,
    ) -> Tuple[dict, Callable[[AgentID], PolicyID]]:
        # The RLlib server must know about the Spaces that the Client will be
        # using inside Unity3D, up-front.
        obs_spaces = {
            "AlanParcour1": TupleSpace(
                [
                    Box(float("-inf"), float("inf"), (26,)),
                    Box(float("-inf"), float("inf"), (3,)),
                ]
            )
        }
        action_spaces = {
            "AlanParcour1": MultiDiscrete([3, 5, 2]),
        }

        policies = {
            game_name: PolicySpec(
                observation_space=obs_spaces[game_name],
                action_space=action_spaces[game_name],
            ),
        }

        def policy_mapping_fn(agent_id, episode, worker, **kwargs):
            return game_name

        return policies, policy_mapping_fn
