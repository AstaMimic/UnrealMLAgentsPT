=========
Changelog
=========

Versions follow `Semantic Versioning <https://semver.org/>`_ (``<major>.<minor>.<patch>``).

Backward incompatible (breaking) changes will only be introduced in major versions
with advance notice in the **Deprecations** section of releases.

.. warning::
    You should *NOT* be adding new change log entries to this file, this
    file is managed by towncrier. You *may* edit previous change logs to
    fix problems like typo corrections or such.

.. towncrier release notes start

UnrealMLAgents 1.0.0 (2025-02-11)
=================================

Features
--------

- Introduced core Unreal ML Agents Plugin to enable reinforcement learning (RL) in Unreal Engine.
- Implemented Python API integration for seamless communication with training workflows.
- Added vector-based observations and RaySensor components for improved perception.
- Provided Blueprint-friendly design, allowing customization without C++.
- Enabled Multi-Agent support for scalable and parallelized training environments.



Known Issues
------------

- Inference is not yet implemented; currently, training is the primary focus.
- SideChannels are not yet supported.
- Imitation Learning is not yet supported.
- Limited sensor support compared to Unity ML-Agents; more sensors will be added in future updates.
- Not all features have been thoroughly tested, and some areas may still contain unresolved issues.
- Lacking proper unit tests; additional validation and automated testing are needed to improve stability.
