from enum import IntEnum

class Command(IntEnum):
    GAME                        = 0x00100000
    
    PLAYER                      = 0x00200000
    
    SOLVER_GUN                  = 0x00400000
    
    FLASHLIGHT                  = 0x00800000
    
    STATIC_MESH                 = 0x01000000
    
    DYNAMIC_MESH                = 0x02000000
    DYNAMIC_MESH_CREATE         = 0x02000000
    DYNAMIC_MESH_SET_MATERIAL   = 0x02000001
    DYNAMIC_MESH_SET_MESH       = 0x02000002
    DYNAMIC_MESH_SET_VISIBILITY = 0x02000003
    
    ELEMENT                     = 0x04000000
    ELEMENT_CREATE              = 0x04000001
    ELEMENT_SET_CALLBACK        = 0x04000002
    ELEMENT_SET_STATE           = 0x04000003
    
    TEST                        = 0xffffffff

class Event(IntEnum):
    NONE            = 0x00000000

    ELEMENT_CHANGED = 0x00000001

class Result(IntEnum):
    SUCCESS                        = 0x00000000
    
    ERROR_COMMAND_NOT_FOUND        = 0x00000001
    ERROR_ARRAY_INDEX_OUT_OF_RANGE = 0x00000002
    ERROR_ID_NOT_FOUND             = 0x00000003
    ERROR_INVALID_OBJECT_TYPE      = 0x00000004

class Mesh(IntEnum):
    PLANE       = 0x00000000
    FACE        = 0x00000001
    CUBE        = 0x00000002
    
    CUP_INNER   = 0x00000003
    CUP_OUTER   = 0x00000004
    II_INNER    = 0x00000005
    II_OUTER    = 0x00000006
    O_INNER     = 0x00000007
    O_OUTER     = 0x00000008
    U_INNER     = 0x00000009
    U_OUTER     = 0x0000000A
    
    DOOR_FRAME  = 0x0000000B
    LASER_FRAME = 0x0000000C

class Material(IntEnum):
    WALL_WHITE_SMALL            = 0x00000000
    WALL_WHITE_MEDIUM           = 0x00000001
    WALL_WHITE_DOUBLE           = 0x00000002
    WALL_WHITE_BIG              = 0x00000003
    WALL_WHITE_ABSOLUTE_SCIENCE = 0x00000004
    
    WALL_BLACK_SMALL            = 0x00000005
    WALL_BLACK_MEDIUM           = 0x00000006
    WALL_BLACK_BIG              = 0x00000007
    
    FLOOR_WHITE                 = 0x00000008
    FLOOR_BLACK                 = 0x00000009

class Element(IntEnum):
    NONE   = 0x0000
    
    DOOR   = 0x0001
    BUTTON = 0x0002
