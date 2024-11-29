class UnrealException(Exception):
    """
    Any error related to ml-agents environment.
    """


class UnrealTimeOutException(UnrealException):
    """
    Related to errors with communication timeouts.
    """


class UnrealWorkerInUseException(UnrealException):
    """
    This error occurs when the port for a certain worker ID is already reserved.
    """

    MESSAGE_TEMPLATE = (
        "Couldn't start socket communication because worker number {} is still in use. "
        "You may need to manually close a previously opened environment "
        "or use a different worker number."
    )

    def __init__(self, worker_id):
        message = self.MESSAGE_TEMPLATE.format(str(worker_id))
        super().__init__(message)


class UnrealEnvironmentException(UnrealException):
    """
    Related to errors starting and closing environment.
    """


class UnrealCommunicationException(UnrealException):
    """
    Related to errors with the communicator.
    """

class UnrealCommunicatorStoppedException(UnrealException):
    """
    Raised when communicator has stopped gracefully.
    """


class UnrealActionException(UnrealException):
    """
    Related to errors with sending actions.
    """


class UnrealObservationException(UnrealException):
    """
    Related to errors with receiving observations.
    """

class UnrealPolicyException(UnrealException):
    """
    Related to errors with the Trainer.
    """
