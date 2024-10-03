import grpc
from typing import Optional, Callable

from multiprocessing import Pipe
from sys import platform
import socket
import time
from concurrent.futures import ThreadPoolExecutor

from ueagents_envs.communicator_objects.unreal_to_external_pb2_grpc import (
    UnrealToExternalProtoServicer,
    add_UnrealToExternalProtoServicer_to_server,
)
from ueagents_envs.communicator_objects.unreal_message_pb2 import UnrealMessageProto
from ueagents_envs.communicator_objects.unreal_input_pb2 import UnrealInputProto
from ueagents_envs.communicator_objects.unreal_output_pb2 import UnrealOutputProto
from ueagents_envs.exception import UnrealTimeOutException, UnrealWorkerInUseException

# Function to call while waiting for a connection timeout.
# This should raise an exception if it needs to break from waiting for the timeout.
PollCallback = Callable[[], None]


class UnrealToExternalServicerImplementation(UnrealToExternalProtoServicer):
    def __init__(self):
        self.parent_conn, self.child_conn = Pipe()

    def Initialize(self, request, context):
        self.child_conn.send(request)
        return self.child_conn.recv()

    def Exchange(self, request, context):
        self.child_conn.send(request)
        return self.child_conn.recv()


class RpcCommunicator:
    def __init__(self, worker_id=0, base_port=5005, timeout_wait=30):
        """
        Python side of the grpc communication. Python is the server and Unreal the client


        :int base_port: Baseline port number to connect to Unreal environment over. worker_id increments over this.
        :int worker_id: Offset from base_port. Used for training multiple environments simultaneously.
        :int timeout_wait: Timeout (in seconds) to wait for a response before exiting.
        """
        self.port = base_port + worker_id
        self.worker_id = worker_id
        self.timeout_wait = timeout_wait
        self.server = None
        self.unreal_to_external = None
        self.is_open = False
        self.create_server()

    def create_server(self):
        """
        Creates the GRPC server.
        """
        self.check_port(self.port)

        try:
            # Establish communication grpc
            self.server = grpc.server(
                thread_pool=ThreadPoolExecutor(max_workers=10),
                options=(("grpc.so_reuseport", 1),),
            )
            self.unreal_to_external = UnrealToExternalServicerImplementation()
            add_UnrealToExternalProtoServicer_to_server(
                self.unreal_to_external, self.server
            )
            # Using unspecified address, which means that grpc is communicating on all IPs
            # This is so that the docker container can connect.
            self.server.add_insecure_port("[::]:" + str(self.port))
            self.server.start()
            self.is_open = True
        except Exception:
            raise UnrealWorkerInUseException(self.worker_id)

    def check_port(self, port):
        """
        Attempts to bind to the requested communicator port, checking if it is already in use.
        """
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if platform == "linux" or platform == "linux2":
            # On linux, the port remains unusable for TIME_WAIT=60 seconds after closing
            # SO_REUSEADDR frees the port right after closing the environment
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            s.bind(("127.0.0.1", port))
        except OSError:
            raise UnrealWorkerInUseException(self.worker_id)
        finally:
            s.close()

    def poll_for_timeout(self, poll_callback: Optional[PollCallback] = None) -> None:
        """
        Polls the GRPC parent connection for data, to be used before calling recv.  This prevents
        us from hanging indefinitely in the case where the environment process has died or was not
        launched.

        Additionally, a callback can be passed to periodically check the state of the environment.
        This is used to detect the case when the environment dies without cleaning up the connection,
        so that we can stop sooner and raise a more appropriate error.
        """
        deadline = time.monotonic() + self.timeout_wait
        callback_timeout_wait = self.timeout_wait // 10
        while time.monotonic() < deadline:
            if self.unreal_to_external.parent_conn.poll(callback_timeout_wait):
                # Got an acknowledgment from the connection
                return
            if poll_callback:
                # Fire the callback - if it detects something wrong, it should raise an exception.
                poll_callback()

        # Got this far without reading any data from the connection, so it must be dead.
        raise UnrealTimeOutException(
            "The Unreal environment took too long to respond. Make sure that :\n"
            "\t The environment does not need user interaction to launch\n"
            '\t The Agents\' Behavior Parameters > Behavior Type is set to "Default"\n'
            "\t The environment and the Python interface have compatible versions.\n"
            "\t If you're running on a headless server without graphics support, turn off display "
            "by either passing --no-graphics option or build your Unreal executable as server build."
        )

    def initialize(
        self, inputs: UnrealInputProto, poll_callback: Optional[PollCallback] = None
    ) -> UnrealOutputProto:
        self.poll_for_timeout(poll_callback)
        aca_param = self.unreal_to_external.parent_conn.recv().unreal_output
        message = UnrealMessageProto()
        message.header.status = 200
        message.unreal_input.CopyFrom(inputs)
        self.unreal_to_external.parent_conn.send(message)
        self.unreal_to_external.parent_conn.recv()
        return aca_param

    def exchange(
        self, inputs: UnrealInputProto, poll_callback: Optional[PollCallback] = None
    ) -> Optional[UnrealOutputProto]:
        message = UnrealMessageProto()
        message.header.status = 200
        message.unreal_input.CopyFrom(inputs)
        self.unreal_to_external.parent_conn.send(message)
        self.poll_for_timeout(poll_callback)
        output = self.unreal_to_external.parent_conn.recv()
        if output.header.status != 200:
            return None
        return output.unreal_output

    def close(self):
        """
        Sends a shutdown signal to the unity environment, and closes the grpc connection.
        """
        if self.is_open:
            message_input = UnrealMessageProto()
            message_input.header.status = 400
            self.unreal_to_external.parent_conn.send(message_input)
            self.unreal_to_external.parent_conn.close()
            self.server.stop(False)
            self.is_open = False
