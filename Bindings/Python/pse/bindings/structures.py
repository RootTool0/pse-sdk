import ctypes
from ctypes import Structure, c_float, c_uint8

class Vector3(Structure):
    _fields_ = [("x", c_float), ("y", c_float), ("z", c_float)]

class Quat(Structure):
    _fields_ = [("x", c_float), ("y", c_float), ("z", c_float), ("w", c_float)]

class Transform(Structure):
    _fields_ = [("quat", Quat), ("location", Vector3), ("scale", Vector3)]

class Color(Structure):
    _fields_ = [("r", c_uint8), ("g", c_uint8), ("b", c_uint8), ("a", c_uint8)]

class BufferData(ctypes.Structure):
    _fields_ = [("raw", c_uint8 * 64)]
