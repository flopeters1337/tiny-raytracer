#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template <size_t DIM, typename T>
struct Vec 
{
    Vec() 
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

typedef Vec<2, float> Vec2f;
typedef Vec<3, float> Vec3f;
typedef Vec<3, int> Vec3i;
typedef Vec<4, float> Vec4f;

template <typename T> struct Vec<2, T>
{
    Vec() : x{T{}}, y{T{}} {}
    Vec(T X, T Y) : x{X}, y{Y} {}
    template <class U> Vec<2, T>(const Vec<2, U>& v);
    
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

template <typename T> struct Vec<3, T>
{
    Vec() : x{T{}}, y{T{}}, z{T{}} {}
    Vec(T X, T Y, T Z) : x{X}, y{Y}, z{Z} {}

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

    float Norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec<3, T> Normalized(T Length = 1) const
    {
        Vec<3, T> NormalizedVector = (*this) * (Length / Norm());
        return NormalizedVector;
    }

    Vec<3, T>& Normalize(T Length = 1)
    {
        *this = (*this) * (Length / Norm());
        return *this;
    }

    T DistanceSquared(const Vec<3, T>& OtherVector) const
    {
        return (OtherVector.x - x) * (OtherVector.x - x) + (OtherVector.y - y) * (OtherVector.y - y) + (OtherVector.z - z) * (OtherVector.z - z);
    }

    T x, y, z;
};

template <typename T> struct Vec<4, T>
{
    Vec() : x{T{}}, y{T{}}, z{T{}}, w{T{}} {}
    Vec(T X, T Y, T Z, T W) : x{X}, y{Y}, z{Z}, w{W} {}

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

template<size_t DIM, typename T> T operator*(const Vec<DIM, T>& LeftVector, const Vec<DIM, T>& RightVector)
{
    T ReturnValue = T{};
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnValue += LeftVector[Index] * RightVector[Index];
    }
    return ReturnValue;
}

template<size_t DIM, typename T> Vec<DIM, T> operator+(Vec<DIM, T> LeftVector, const Vec<DIM, T>& RightVector)
{
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        LeftVector[Index] += RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T> Vec<DIM, T> operator-(Vec<DIM, T> LeftVector, const Vec<DIM, T>& RightVector)
{
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        LeftVector[Index] -= RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T, typename U> Vec<DIM, T> operator*(const Vec<DIM, T> LeftVector, const U& RightValue)
{
    Vec<DIM, T> ReturnVector;
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnVector[Index] = LeftVector[Index] * RightValue;
    }
    return ReturnVector;
}

template <size_t DIM, typename T> Vec<DIM, T> operator-(const Vec<DIM, T>& LeftVector)
{
    return LeftVector * T{-1};
}

template <size_t DIM, typename T> T Dot(Vec<DIM, T> VectorA, Vec<DIM, T> VectorB)
{
    T ReturnValue = T{0};
    for (size_t Index = 0; Index < DIM; ++Index)
    {
        ReturnValue += VectorA[Index] * VectorB[Index];
    }
    return ReturnValue;
}

template <typename T> Vec<3, T> Cross(Vec<3, T> VectorA, Vec<3, T> VectorB)
{
    return Vec<3, T>{VectorA.y * VectorB.z - VectorA.z * VectorB.y, VectorA.z * VectorB.x - VectorA.x * VectorB.z, VectorA.x * VectorB.y - VectorA.y * VectorB.x};
}

template <typename T> Vec<3, T> Project(const Vec<3, T>& Point, const Vec<3, T>& RayOrigin, const Vec<3, T>& RayDirection)
{
    const Vec<3, T> RayOriginToPoint = Point - RayOrigin;
    const Vec<3, T> NormalizedDirection = RayDirection.Normalized();
    const float DotProduct = Dot(RayOriginToPoint, NormalizedDirection);
    const Vec<3, T> OriginToProjection =  NormalizedDirection * DotProduct;
    const Vec<3, T> Projection = RayOrigin + OriginToProjection;
    return Projection;
};

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& Out, const Vec<DIM, T>& Vector)
{
    for (unsigned int Index = 0; Index < DIM; ++Index)
    {
        Out << Vector[Index] << " ";
    }
    return Out;
}

#endif //__GEOMETRY_H__