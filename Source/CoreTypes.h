#pragma once

struct PseVector
{
    float X, Y, Z;
    
    PseVector() = default;
    explicit PseVector(float InF) : X(InF), Y(InF), Z(InF) {}
    explicit PseVector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}
};

struct PseQuat
{
    float X, Y, Z, W;
    
    PseQuat() = default;
    explicit PseQuat(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
};

struct PseTransform
{
    PseQuat Quat;
    PseVector Location;
    PseVector Scale;
    
    PseTransform() 
        : Quat(0, 0, 0, 1)
        , Location(0, 0, 0)
        , Scale(1, 1, 1) {}
    
    explicit PseTransform(const PseVector& InLocation) 
        : Quat(0, 0, 0, 1)
        , Location(InLocation)
        , Scale(1, 1, 1) {}
    
    explicit PseTransform(const PseQuat& InQuat) 
        : Quat(InQuat)
        , Location(0, 0, 0)
        , Scale(1, 1, 1) {}
    
    explicit PseTransform(const PseVector& InLoc, const PseQuat& InQuat, const PseVector& InScale = PseVector(1.0f))
        : Quat(InQuat)
        , Location(InLoc)
        , Scale(InScale) {}
};
