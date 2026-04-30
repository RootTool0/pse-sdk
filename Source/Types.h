#pragma once
#include <cstdint>
#include <cstring>

using PseID = uint64_t;

enum PseCommand : uint32_t
{
	PSE_COMMAND_DYNAMIC_MESH_CREATE         = 0x00000000,
	PSE_COMMAND_DYNAMIC_MESH_SET_MATERIAL   = 0x00000001,
	PSE_COMMAND_DYNAMIC_MESH_SET_MESH       = 0x00000002,
	PSE_COMMAND_DYNAMIC_MESH_SET_VISIBILITY = 0x00000003
};

enum PseEvent : uint32_t
{
	
};

enum PseResult : uint32_t
{
	PSE_RESULT_SUCCESS = 0x00000000,
	
	PSE_RESULT_ERROR_COMMAND_NOT_FOUND        = 0x00000001,
	PSE_RESULT_ERROR_ARRAY_INDEX_OUT_OF_RANGE = 0x00000002,
	PSE_RESULT_ERROR_ID_NOT_FOUND             = 0x00000003,
	PSE_RESULT_ERROR_INVALID_OBJECT_TYPE      = 0x00000004
};

struct alignas(64) PseData
{
	union
	{
		PseCommand Command;
		PseEvent Event;
		PseResult Result;
	};
	
	uint8_t Data[60];
	
	template<int32_t Offset, typename T>
	T Get() const { return *reinterpret_cast<const T*>(&Data[Offset]); }
	
	template<int32_t Offset, typename T>
	PseData& Set(const T& InData) { std::memcpy(&Data[Offset], &InData, sizeof(T)); return *this; }
	
	static PseData CreateCommand(PseCommand PseCommand) { PseData PseData; PseData.Command = PseCommand; return PseData; }
	static PseData CreateResult(PseResult PseResult) { PseData PseData; PseData.Result = PseResult; return PseData; }
};

enum PseMesh : uint32_t
{
	PSE_MESH_PLANE = 0x00000000,
	PSE_MESH_FACE  = 0x00000001,
	PSE_MESH_CUBE  = 0x00000002,
	
	PSE_MESH_CUP_INNER = 0x00000003,
	PSE_MESH_CUP_OUTER = 0x00000004,
	PSE_MESH_II_INNER  = 0x00000005,
	PSE_MESH_II_OUTER  = 0x00000006,
	PSE_MESH_O_INNER   = 0x00000007,
	PSE_MESH_O_OUTER   = 0x00000008,
	PSE_MESH_U_INNER   = 0x00000009,
	PSE_MESH_U_OUTER   = 0x0000000A,
	
	PSE_MESH_DOOR_FRAME  = 0x0000000B,
	PSE_MESH_LASER_FRAME = 0x0000000C,
};

enum PseMaterial : uint32_t
{
	PSE_MATERIAL_WALL_WHITE_SMALL            = 0x00000000,
	PSE_MATERIAL_WALL_WHITE_MEDIUM           = 0x00000001,
	PSE_MATERIAL_WALL_WHITE_DOUBLE           = 0x00000002,
	PSE_MATERIAL_WALL_WHITE_BIG              = 0x00000003,
	PSE_MATERIAL_WALL_WHITE_ABSOLUTE_SCIENCE = 0x00000004,
	
	PSE_MATERIAL_WALL_BLACK_SMALL  = 0x00000005,
	PSE_MATERIAL_WALL_BLACK_MEDIUM = 0x00000006,
	PSE_MATERIAL_WALL_BLACK_BIG    = 0x00000007,
	
	PSE_MATERIAL_FLOOR_WHITE = 0x00000008,
	PSE_MATERIAL_FLOOR_BLACK = 0x00000009,
};
