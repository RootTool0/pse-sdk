#include <cstdio>
#include <random>

#include "pse.h"
#include "utilities.hpp"

PseGUID button;

void onButtonChanged(int8_t inState)
{
    static std::minstd_rand gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(-400, 400);
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_SET_TRANSFORM;
        PseUtilities::BufferData::set<0, PseGUID>(bufferData, button);
        PseUtilities::BufferData::set<8, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{static_cast<float>(dist(gen)), static_cast<float>(dist(gen)), -100}, PseVector3{1, 1, 1}});
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        printf("%x\n", bufferData.header);
    }
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
    
    for(int x = 0; x < 5; ++x)
        for(int y = 0; y < 5; ++y)
        {
            PseBufferData bufferData{};
            bufferData.header = PSE_COMMAND_DYNAMIC_MESH_CREATE;
            PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{static_cast<float>(x) * 200 - 400, static_cast<float>(y) * 200 - 400, -100}, PseVector3{1, 1, 1}});
            PseUtilities::BufferData::set<40, PseMesh>(bufferData, PSE_MESH_FACE);
            PseUtilities::BufferData::set<42, PseMaterial>(bufferData, (x + y) % 2 == 0 ? PSE_MATERIAL_FLOOR_WHITE : PSE_MATERIAL_FLOOR_BLACK);
            PseUtilities::BufferData::set<44, int8_t>(bufferData, 1);
            
            pseCommandBufferSinglePush(&bufferData);
        }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{200, 200, -100}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_BUTTON);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, (uint64_t)&onButtonChanged);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        button = PseUtilities::BufferData::get<0, PseGUID>(bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_ELEMENT_CREATE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{100, -100, 0}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_WEIGHT_CUBE);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, 0);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
    }
    
    {
        PseBufferData bufferData{};
        bufferData.header = PSE_COMMAND_REPLACE;
        PseUtilities::BufferData::set< 0, PseTransform>(bufferData, PseTransform{PseQuat{}, PseVector3{100, -100, 0}, PseVector3{1, 1, 1}});
        PseUtilities::BufferData::set<40, PseClass>(bufferData, PSE_CLASS_WEIGHT_CUBE);
        PseUtilities::BufferData::set<42, uint64_t>(bufferData, 0);
        PseUtilities::BufferData::set<50, int8_t>(bufferData, 0);
        PseUtilities::BufferData::set<51, int8_t>(bufferData, 1);
        
        printf("\n%02X\n", bufferData.header);
        for(size_t i = 0; i < 60; i++) {
            printf("%02X ", bufferData.data[i]);
        }

        pseCommandBufferSinglePushAndWait(&bufferData);

        printf("\n%02X\n", bufferData.header);
        for(size_t i = 0; i < 60; i++) {
            printf("%02X ", bufferData.data[i]);
        }
        
        /*
        printf("\n%02X\n", bufferData.header);
        for(uint8_t b : bufferData.data) printf("%02X ", b);
        
        pseCommandBufferSinglePushAndWait(&bufferData);
        
        printf("\n%02X\n", bufferData.header);
        for(uint8_t b : bufferData.data) printf("%02X ", b);
        */
        
    }
    
    while(true)
    {
        PseBufferData bufferData{};
        if(pseEventBufferTryGet(&bufferData) != 0)
        {
            if(bufferData.header == PSE_EVENT_ELEMENT_CHANGED)
            {
                uint64_t callback = PseUtilities::BufferData::get< 8, uint64_t>(bufferData);
                int8_t state = PseUtilities::BufferData::get<16, int8_t>(bufferData);
                if(callback != 0) reinterpret_cast<void(*)(int8_t)>(callback)(state);
            }
        }
    }
    
    pseDeinitializeBuffers();
    return 0;
}
