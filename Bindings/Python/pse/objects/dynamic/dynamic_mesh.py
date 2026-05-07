from pse import GUID, Command, Mesh, Material, Vector3, Quat, Transform
from pse import globals
from pse.bindings import BufferData
from ctypes import byref

class DynamicMesh:
    def __init__(self):
        self._guid: GUID = GUID(0)

    @property
    def guid(self) -> GUID:
        return self._guid

    def create(self, mesh=Mesh.PLANE, material=Material.WALL_WHITE_SMALL, quat=(0, 0, 0, 0), location=(0, 0, 0), scale=(1, 1, 1)):
        data = BufferData()
        globals.dll.pseBufferDataCreateCommand(byref(data), Command.DYNAMIC_MESH_CREATE)
        globals.dll.pseBufferDataSetInt32(byref(data), 0, mesh)
        globals.dll.pseBufferDataSetInt32(byref(data), 4, material)
        transform = Transform(Quat(*quat), Vector3(*location), Vector3(*scale))
        globals.dll.pseBufferDataSetTransform(byref(data), 8, byref(transform))
        
        globals.dll.pseCommandBufferSinglePushAndWait(byref(data))
        self._guid = globals.dll.pseBufferDataGetGUID(byref(data), 0)
        
    def set_material(self, material: Material):
        pass

    def set_mesh(self, mesh_type: Mesh):
        pass

    def set_visibility(self, visible: bool):
        pass
