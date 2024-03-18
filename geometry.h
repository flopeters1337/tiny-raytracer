#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template <size_t DIM, typename T>
struct SVec 
{
    SVec() 
    { 
        for (size_t Index = DIM; Index > 0; --Index)
        {
            Data[Index] = T{};
        }
    }

    T& operator[](const size_t Index)
    {
        assert(Index < DIM);
        return Data[Index];
    }

    const T& operator[](const size_t Index) const 
    {
        assert(Index < DIM);
        return Data[Index];
    }

private:
    T Data[DIM];
};

typedef SVec<2, float> SVec2f;
typedef SVec<3, float> SVec3f;
typedef SVec<3, int> SVec3i;
typedef SVec<4, float> SVec4f;

template <typename T> struct SVec<2, T>
{
    SVec() : x{T{}}, y{T{}} {}
    SVec(T X, T Y) : x{X}, y{Y} {}
    template <class U> SVec<2, T>(const SVec<2, U>& v);
    
    T& operator[](const size_t Index)
    {
        assert(Index < 2);
        return Index <= 0 ? x : y;
    }

    const T& operator[](const size_t Index) const
    {
        assert(Index < 2);
        return Index <= 0 ? x : y;
    }

    T x, y;
};

template <typename T> struct SVec<3, T>
{
    SVec() : x{T{}}, y{T{}}, z{T{}} {}
    SVec(T X, T Y, T Z) : x{X}, y{Y}, z{Z} {}

    T& operator[](const size_t Index)
    {
        assert(Index < 3);
        return Index <= 0 ? x : (Index == 1 ? y : z);
    }

    const T& operator[](const size_t Index) const
    {
        assert(Index < 3);
        return Index <= 0 ? x : (Index == 1 ? y : z);
    }

    bool operator==(const SVec<3, T>& OtherVector) const
    {
        return x == OtherVector.x && y == OtherVector.y && z == OtherVector.z;
    }
    
    bool operator!=(const SVec<3, T>& OtherVector) const
    {
        return x != OtherVector.x && y != OtherVector.y && z != OtherVector.z;
    }

    float Norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    SVec<3, T> Normalized(T Length = 1) const
    {
        SVec<3, T> NormalizedVector = (*this) * (Length / Norm());
        return NormalizedVector;
    }

    SVec<3, T>& Normalize(T Length = 1)
    {
        *this = (*this) * (Length / Norm());
        return *this;
    }

    T DistanceSquared(const SVec<3, T>& OtherVector) const
    {
        return (OtherVector.x - x) * (OtherVector.x - x) + (OtherVector.y - y) * (OtherVector.y - y) + (OtherVector.z - z) * (OtherVector.z - z);
    }

    bool NearlyEqual(const SVec<3, T>& OtherVector, const float Tolerance = 1e-3) const
    {
        return DistanceSquared(OtherVector) <= Tolerance * Tolerance;
    }

    T x, y, z;
};

template <typename T> struct SVec<4, T>
{
    SVec() : x{T{}}, y{T{}}, z{T{}}, w{T{}} {}
    SVec(T X, T Y, T Z, T W) : x{X}, y{Y}, z{Z}, w{W} {}

    T& operator[](const size_t Index)
    {
        assert(Index < 4);
        return Index <= 0 ? x : (Index == 1 ? y : (Index == 2 ? z : w));
    }

    const T& operator[](const size_t Index) const
    {
        assert(Index < 4);
        return Index <= 0 ? x : (Index == 1 ? y : (Index == 2 ? z : w));
    }

    T x, y, z, w;
};

template<size_t DIM, typename T> T operator*(const SVec<DIM, T>& LeftVector, const SVec<DIM, T>& RightVector)
{
    T ReturnValue = T{};
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnValue += LeftVector[Index] * RightVector[Index];
    }
    return ReturnValue;
}

template<size_t DIM, typename T> SVec<DIM, T> operator+(SVec<DIM, T> LeftVector, const SVec<DIM, T>& RightVector)
{
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        LeftVector[Index] += RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T> SVec<DIM, T> operator-(SVec<DIM, T> LeftVector, const SVec<DIM, T>& RightVector)
{
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        LeftVector[Index] -= RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T, typename U> SVec<DIM, T> operator*(const SVec<DIM, T> LeftVector, const U& RightValue)
{
    SVec<DIM, T> ReturnVector;
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnVector[Index] = LeftVector[Index] * RightValue;
    }
    return ReturnVector;
}

template<size_t DIM, typename T, typename U> SVec<DIM, T> operator*(const U& LeftValue, const SVec<DIM, T> RightVector)
{
    SVec<DIM, T> ReturnVector;
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnVector[Index] = LeftValue * RightVector[Index];
    }
    return ReturnVector;
}

template <size_t DIM, typename T> SVec<DIM, T> operator-(const SVec<DIM, T>& LeftVector)
{
    return LeftVector * T{-1};
}

template <size_t DIM, typename T> T Dot(const SVec<DIM, T>& VectorA, const SVec<DIM, T>& VectorB)
{
    T ReturnValue = T{0};
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnValue += VectorA[Index] * VectorB[Index];
    }
    return ReturnValue;
}

template <typename T> SVec<3, T> Cross(SVec<3, T> VectorA, SVec<3, T> VectorB)
{
    return SVec<3, T>{VectorA.y * VectorB.z - VectorA.z * VectorB.y, VectorA.z * VectorB.x - VectorA.x * VectorB.z, VectorA.x * VectorB.y - VectorA.y * VectorB.x};
}

template <typename T> SVec<3, T> Project(const SVec<3, T>& Point, const SVec<3, T>& RayOrigin, const SVec<3, T>& RayDirection)
{
    const SVec<3, T> RayOriginToPoint = Point - RayOrigin;
    const SVec<3, T> NormalizedDirection = RayDirection.Normalized();
    const float DotProduct = Dot(RayOriginToPoint, NormalizedDirection);
    const SVec<3, T> OriginToProjection =  NormalizedDirection * DotProduct;
    const SVec<3, T> Projection = RayOrigin + OriginToProjection;
    return Projection;
};

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& Out, const SVec<DIM, T>& Vector)
{
    for (unsigned int Index = 0; Index < DIM; ++Index)
    {
        Out << Vector[Index] << " ";
    }
    return Out;
}

#endif //__GEOMETRY_H__