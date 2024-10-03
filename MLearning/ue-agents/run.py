import argparse
import os

import ray
from ray import air, tune
from ray.air.constants import TRAINING_ITERATION
from ray.rllib.algorithms.ppo import PPOConfig
from ray.rllib.utils.metrics import (
    ENV_RUNNER_RESULTS,
    NUM_ENV_STEPS_SAMPLED_LIFETIME,
)
from ray.rllib.utils.test_utils import check_learning_achieved

from envs.unreal_ray_rl import Unreal3DEnv
from ueagents_envs.logging_util import set_log_level, DEBUG, INFO

set_log_level(DEBUG)

parser = argparse.ArgumentParser()
parser.add_argument(
    "--env",
    type=str,
    default="AlanParcour1",
    choices=[
        "AlanParcour1",
    ],
    help="The name of the Env to run in the Unreal3D editor",
)
parser.add_argument(
    "--file-name",
    type=str,
    default=None,
    help="The Unreal3d binary (compiled) game, e.g. "
    "'/home/ubuntu/alan.x86_64'. Use `None` for "
    "a currently running Unreal3D editor.",
)
parser.add_argument(
    "--from-checkpoint",
    type=str,
    default=None,
    help="Full path to a checkpoint file for restoring a previously saved "
    "Algorithm state.",
)
parser.add_argument("--num-workers", type=int, default=0)
parser.add_argument(
    "--as-test",
    action="store_true",
    help="Whether this script should be run as a test: --stop-reward must "
    "be achieved within --stop-timesteps AND --stop-iters.",
)
parser.add_argument(
    "--stop-iters", type=int, default=9999, help="Number of iterations to train."
)
parser.add_argument(
    "--stop-timesteps", type=int, default=10000000, help="Number of timesteps to train."
)
parser.add_argument(
    "--stop-reward",
    type=float,
    default=9999.0,
    help="Reward at which we stop training.",
)
parser.add_argument(
    "--horizon",
    type=int,
    default=500000,
    help="The max. number of `step()`s for any episode (per agent) before "
    "it'll be reset again automatically.",
)
parser.add_argument(
    "--framework",
    choices=["tf", "tf2", "torch"],
    default="torch",
    help="The DL framework specifier.",
)

# ---------------------------------------------------------------------------- #
#                                  Core script                                 #
# ---------------------------------------------------------------------------- #

ray.init()

args = parser.parse_args()

tune.register_env(
    "unreal3d",
    lambda c: Unreal3DEnv(
        file_name=c["file_name"],
        no_graphics=(args.env != "VisualHallway" and c["file_name"] is not None),
        episode_horizon=c["episode_horizon"],
    ),
)

# Get policies (different agent types; "behaviors" in MLAgents) and
# the mappings from individual agents to Policies.
policies, policy_mapping_fn = Unreal3DEnv.get_policy_configs_for_game(args.env)

config = (
    PPOConfig()
    .environment(
        "unreal3d",
        env_config={
            "file_name": args.file_name,
            "episode_horizon": args.horizon,
        },
    )
    .framework("tf")
    # For running in editor, force to use just one Worker (we only have
    # one Unity running)!
    .env_runners(
        num_env_runners=args.num_workers if args.file_name else 0,
        rollout_fragment_length=200,
    )
    .training(
        lr=0.0003,
        lambda_=0.95,
        gamma=0.99,
        sgd_minibatch_size=256,
        train_batch_size=4000,
        num_sgd_iter=20,
        clip_param=0.2,
        model={"fcnet_hiddens": [512, 512]},
    )
    .multi_agent(policies=policies, policy_mapping_fn=policy_mapping_fn)
    # Use GPUs iff `RLLIB_NUM_GPUS` env var set to > 0.
    .resources(num_gpus=int(os.environ.get("RLLIB_NUM_GPUS", "0")))
)

stop = {
    TRAINING_ITERATION: args.stop_iters,
    NUM_ENV_STEPS_SAMPLED_LIFETIME: args.stop_timesteps,
    f"{ENV_RUNNER_RESULTS}/episode_return_mean": args.stop_reward,
}

# Run the experiment.
results = tune.Tuner(
    "PPO",
    param_space=config.to_dict(),
    run_config=air.RunConfig(
        stop=stop,
        verbose=1,
        checkpoint_config=air.CheckpointConfig(
            checkpoint_frequency=5,
            checkpoint_at_end=True,
        ),
    ),
).fit()

# And check the results.
if args.as_test:
    check_learning_achieved(results, args.stop_reward)

ray.shutdown()
