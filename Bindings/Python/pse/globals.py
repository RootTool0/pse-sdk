from ctypes import CFUNCTYPE, c_int8

dll = None
CALLBACK_TYPE = CFUNCTYPE(None, c_int8)
