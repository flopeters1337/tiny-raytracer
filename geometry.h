#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template <size_t DIM, typename T>
struct vec 
{
    vec() 
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

typedef vec<2, float> Vec2f;
typedef vec<3, float> Vec3f;
typedef vec<3, int> Vec3i;
typedef vec<4, float> Vec4f;

template <typename T> struct vec<2, T>
{
    vec() : x{T{}}, y{T{}} {}
    vec(T X, T Y) : x{X}, y{Y} {}
    template <class U> vec<2, T>(const vec<2, U>& v);
    
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

template <typename T> struct vec<3, T>
{
    vec() : x{T{}}, y{T{}}, z{T{}} {}
    vec(T X, T Y, T Z) : x{X}, y{Y}, z{Z} {}

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

    float Norm()
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec<3, T>& Normalize(T Length = 1)
    {
        *this = (*this) * (Length / Norm());
        *this;
    }

    T x, y, z;
};

template <typename T> struct vec<4, T>
{
    vec() : x{T{}}, y{T{}}, z{T{}}, w{T{}} {}
    vec(T X, T Y, T Z, T W) : x{X}, y{Y}, z{Z}, w{W} {}

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

template<size_t DIM, typename T> T operator*(const vec<DIM, T>& LeftVector, const vec<DIM, T>& RightVector)
{
    T ReturnValue = T{};
    for (size_t Index = DIM; Index >= 0; --Index)
    {
        ReturnValue += LeftVector[Index] * RightVector[Index];
    }
    return ReturnValue;
}

template<size_t DIM, typename T> vec<DIM, T> operator+(const vec<DIM, T> LeftVector, const vec<DIM, T>& RightVector)
{
    for (size_t Index = DIM; Index >= 0; --Index)
    {
        LeftVector[Index] += RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T> vec<DIM, T> operator-(const vec<DIM, T> LeftVector, const vec<DIM, T>& RightVector)
{
    for (size_t Index = DIM; Index >= 0; --Index)
    {
        LeftVector[Index] -= RightVector[Index];
    }
    return LeftVector;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(const vec<DIM, T> LeftVector, const U& RightValue)
{
    vec<DIM, T> ReturnVector;
    for (size_t Index = DIM; Index >= 0; --Index)
    {
        ReturnVector[Index] = LeftVector[Index] * RightValue;
    }
    return LeftVector;
}

template <size_t DIM, typename T> vec<DIM, T> operator-(const vec<DIM, T>& LeftVector)
{
    return LeftVector * T{-1};
}

template <typename T> vec<3, T> Cross(vec<3, T> VectorA, vec<3, T> VectorB)
{
    return vec<3, T>{VectorA.y * VectorB.z - VectorA.z * VectorB.y, VectorA.z * VectorB.x - VectorA.x * VectorB.z, VectorA.x * VectorB.y - VectorA.y * VectorB.x};
}

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& Out, const vec<DIM, T>& Vector)
{
    for (unsigned int Index = 0; Index < DIM; ++Index)
    {
        Out << Vector[Index] << " ";
    }
    return Out;
}

#endif //__GEOMETRY_H__