from ctypes import POINTER, c_int32, c_int8, c_int16, c_int64, c_uint32, c_uint64
from .structures import Vector3, Quat, Transform, Color, BufferData
from pse import globals

def setup_functions():
    functions_table = {
        # PseBufferData / Create
        "pseBufferDataCreateCommand": (None, [POINTER(BufferData), c_int32]),
        "pseBufferDataCreateResult": (None, [POINTER(BufferData), c_int32]),
        "pseBufferDataCreateEvent": (None, [POINTER(BufferData), c_int32]),

        # PseBufferData / Setters
        "pseBufferDataSetInt8": (None, [POINTER(BufferData), c_int32, c_int8]),
        "pseBufferDataSetInt16": (None, [POINTER(BufferData), c_int32, c_int16]),
        "pseBufferDataSetInt32": (None, [POINTER(BufferData), c_int32, c_int32]),
        "pseBufferDataSetInt64": (None, [POINTER(BufferData), c_int32, c_int64]),
        "pseBufferDataSetGUID": (None, [POINTER(BufferData), c_int32, c_uint64]),

        "pseBufferDataSetVector3": (None, [POINTER(BufferData), c_int32, Vector3]),
        "pseBufferDataSetQuat": (None, [POINTER(BufferData), c_int32, Quat]),
        "pseBufferDataSetTransform": (None, [POINTER(BufferData), c_int32, POINTER(Transform)]),
        "pseBufferDataSetColor": (None, [POINTER(BufferData), c_int32, Color]),

        # PseBufferData / Getters
        "pseBufferDataGetInt8": (c_int8, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetInt16": (c_int16, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetInt32": (c_int32, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetInt64": (c_int64, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetGUID": (c_uint64, [POINTER(BufferData), c_int32]),

        "pseBufferDataGetVector3": (Vector3, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetQuat": (Quat, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetTransform": (Transform, [POINTER(BufferData), c_int32]),
        "pseBufferDataGetColor": (Color, [POINTER(BufferData), c_int32]),

        # Buffers / Basic
        "pseInitializeBuffers": (c_int32, []),
        "pseDeinitializeBuffers": (None, []),

        # Buffers / Address Getters
        # "pseGetBuffersAddress":       (c_void_p, []),
        # "pseGetCommandBufferAddress": (c_void_p, []),
        # "pseGetEventBufferAddress":   (c_void_p, []),
        # "pseGetSharedMemoryHandle":   (c_void_p, []),

        # Command Buffer / Methods
        "pseCommandBufferSynchronize": (None, []),
        "pseCommandBufferSinglePush": (c_uint32, [POINTER(BufferData)]),
        "pseCommandBufferSingleWait": (None, [c_uint32, POINTER(BufferData)]),
        "pseCommandBufferSinglePushAndWait": (None, [POINTER(BufferData)]),

        "pseCommandBufferBatchPush": (c_uint32, [POINTER(BufferData), c_uint32]),
        "pseCommandBufferBatchWait": (None, [c_uint32, POINTER(BufferData), c_uint32]),
        "pseCommandBufferBatchPushAndWait": (None, [POINTER(BufferData), c_uint32]),

        "pseCommandBufferRawReserve": (c_uint32, [c_uint32]),
        "pseCommandBufferRawCommit": (None, [c_uint32, c_uint32]),
        "pseCommandBufferRawWait": (None, [c_uint32, c_uint32]),
        "pseCommandBufferRawCommitAndWait": (None, [c_uint32, c_uint32]),

        # Event Buffer / Methods
        "pseEventBufferTryGet": (c_int32, [POINTER(BufferData)]),
    }

    for name, (restype, argtypes) in functions_table.items():
        func = getattr(globals.dll, name)
        func.restype = restype
        func.argtypes = argtypes
