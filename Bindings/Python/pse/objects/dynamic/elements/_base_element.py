from pse import GUID, Command, Vector3, Quat, Transform
from pse import globals
from pse.bindings import Element, BufferData
from ctypes import byref, cast, c_void_p, c_int8, c_int64, c_uint64

class BaseElement:
    def __init__(self):
        self._guid = GUID(0)
        self._callback = None

    def _get_class(self) -> Element: return Element.NONE

    def create(self, quat=(0, 0, 0, 0), location=(0, 0, 0), scale=(1, 1, 1), visibility=True):
        data = BufferData()
        globals.dll.pseBufferDataCreateCommand(byref(data), Command.ELEMENT_CREATE)
        transform = Transform(Quat(*quat), Vector3(*location), Vector3(*scale))
        globals.dll.pseBufferDataSetTransform(byref(data), 0, byref(transform))
        globals.dll.pseBufferDataSetInt16(byref(data), 40, self._get_class())
        globals.dll.pseBufferDataSetInt8(byref(data), 42, 1 if visibility else 0)

        globals.dll.pseCommandBufferSinglePushAndWait(byref(data))
        self._guid = globals.dll.pseBufferDataGetGUID(byref(data), 0)

    def on_element_changed(self):
        def wrapper(func):
            self._callback = globals.CALLBACK_TYPE(func)
            addr = cast(self._callback, c_void_p).value

            data = BufferData()
            globals.dll.pseBufferDataCreateCommand(byref(data), Command.ELEMENT_SET_CALLBACK)
            globals.dll.pseBufferDataSetGUID(byref(data), 0, c_uint64(self._guid))
            globals.dll.pseBufferDataSetInt64(byref(data), 8, c_int64(addr))

            globals.dll.pseCommandBufferSinglePushAndWait(byref(data))
            return func

        return wrapper

    def set_on_element_changed(self, func):
        return self.on_element_changed()(func)

    def set_state(self, value: int):
        data = BufferData()
        globals.dll.pseBufferDataCreateCommand(byref(data), Command.ELEMENT_SET_STATE)
        globals.dll.pseBufferDataSetGUID(byref(data), 0, c_uint64(self._guid))
        globals.dll.pseBufferDataSetInt8(byref(data), 8, c_int8(value))

        globals.dll.pseCommandBufferSinglePushAndWait(byref(data))
