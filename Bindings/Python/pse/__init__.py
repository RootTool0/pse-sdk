from .bindings import (
    GUID,
    Command, Event, Result, Mesh, Material, Element,
    Vector3, Quat, Transform, Color, BufferData
)
from . import globals
from .objects.dynamic.dynamic_mesh import DynamicMesh
from .objects.dynamic.elements.button_element import ButtonElement
from .objects.dynamic.elements.door_element import DoorElement
from ctypes import byref

def initialize(dll_path: str = None):
    from .bindings import setup_functions
    from os import path
    from ctypes import CDLL

    if dll_path is None: dll_path = "PSE.dll" # TODO: Find Library
    print(path.abspath(dll_path))

    try: globals.dll = CDLL(dll_path)
    except OSError as e: raise RuntimeError(f"Не удалось загрузить DLL по пути: {dll_path}\n{e}")

    setup_functions()

    return globals.dll.pseInitializeBuffers()

def deinitialize():
    if globals.dll is None: return
    globals.dll.pseDeinitializeBuffers()
    globals.dll = None

def poll_events():
    data = BufferData()
    for _ in range(128):
        if globals.dll.pseEventBufferTryGet(byref(data)) == 0: break

        event = globals.dll.pseBufferDataReadEvent(byref(data))
        if event == Event.ELEMENT_CHANGED:
            callback_address = globals.dll.pseBufferDataGetInt64(byref(data), 9)
            if callback_address != 0:
                value = globals.dll.pseBufferDataGetInt8(byref(data), 8)
                callback = globals.CALLBACK_TYPE(callback_address)
                callback(value)
