#include <csignal>
#include <cstdio>
#include <iostream>

#include "pse.h"
#include "utilities.hpp"

PseGUID lamp;
PseGUID door;
PseGUID button;

void onButtonChanged(int8_t inState)
{
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_SET_STATE;
        PseUtilities::BufferData::set<0, PseGUID>(bufferData, lamp);
        PseUtilities::BufferData::set<8, int8_t>(bufferData, inState);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_SET_STATE;
        PseUtilities::BufferData::set<0, PseGUID>(bufferData, door);
        PseUtilities::BufferData::set<8, int8_t>(bufferData, inState);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
    }
    
    printf("Got State: %i\n", inState);
}

int main()
{
    if(pseInitializeBuffers() != 0)
    {
        printf("Failed to initialize buffers!\n");
        return -1;
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = 0x00100008;
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        if(bufferData.header != PSE_RESULT_SUCCESS)
        {
            printf("Failed to initialize! Error: 0x%08X\n", bufferData.header);
            return -1;
        }
    }
    
    for(int x = 0; x < 10; ++x)
        for(int y = 0; y < 10; ++y)
        {
            PseBufferData bufferData{};
            bufferData.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
            PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{static_cast<float>(x) * 200, static_cast<float>(y) * 200, -100}, PseVector3{1, 1, 1}});
            PseUtilities::BufferData::set<40, PseMesh>(bufferData, PSE_MESH_FACE);
            PseUtilities::BufferData::set<42, PseMaterial>(bufferData, (x + y) % 2 == 0 ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
            PseUtilities::BufferData::set<44, int8_t>(bufferData, 1);
            
            pseCommandBufferSinglePush(&bufferData);
        }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{0.f, 0.f, 1.f, 0.f}, PseVector3{375, 0, -100}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseMesh>(bufferData, PSE_MESH_DOOR_FRAME);
        PseUtilities::BufferData::set<42, PseMaterial>(bufferData, PSE_MATERIAL_WALL_BLACK_SMALL);
        PseUtilities::BufferData::set<44, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePush(&bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{325, 0, -100}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_LAMP);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, 0);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        lamp = PseUtilities::BufferData::get<0, PseGUID>(bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{0.f, 0.f, 1.f, 0.f}, PseVector3{375, 0, -100}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_DOOR);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, 0);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        door = PseUtilities::BufferData::get<0, PseGUID>(bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{200, 0, -100}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_BUTTON);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, (uint64_t)&onButtonChanged);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        button = PseUtilities::BufferData::get<0, PseGUID>(bufferData);
    }
    
    // int i;
    // std::cin >> i;
    
    while(true)
    {
        PseBufferData bufferData{};
        if(pseEventBufferTryGet(&bufferData) != 0)
        {
            uint64_t callback = PseUtilities::BufferData::get< 8, uint64_t>(bufferData);
            int8_t state = PseUtilities::BufferData::get<16, int8_t>(bufferData);
            if(callback != 0) reinterpret_cast<void(*)(int8_t)>(callback)(state);
            printf("Got State!\n");
        }
    }
    
    /*
    bool b = false;
    while(true)
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_SET_STATE;
        PseUtilities::BufferData::set<0, PseGUID>(bufferData, lamp);
        PseUtilities::BufferData::set<8, int8_t>(bufferData, b ? 1 : 0);
        b = !b;
        
        pseCommandBufferSinglePushAndWait(&bufferData);
    }
    */
    
    pseDeinitializeBuffers();
    return 0;
}
