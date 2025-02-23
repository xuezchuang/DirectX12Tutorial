﻿#pragma once

#include <math.h>
#include <stdint.h>
#include <intrin.h>
#include <functional>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>
#include <chrono>

const float MATH_PI = 3.141592654f;
const float MATH_2PI = 2.f * MATH_PI;
const float MATH_PI_HALF = 0.5f * MATH_PI;

struct FBoundingBox;

template<typename T>
struct Vector4;

template<typename T>
struct Vector2
{
	union 
	{
		T data[2];
		struct { T x, y;};
	};
	
	T& operator [] (int i)
	{
		return data[i];
	}
	Vector2() : x(0), y(0) {}
	Vector2(T c) : x(c), y(c) {}
	Vector2(T x, T y) : x(x), y(y) {}

	template<typename V>
	static Vector2 MakeVector(const V* Values)
	{
		return Vector2((T)Values[0], (T)Values[1]);
	}

	Vector2 operator - (const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}
};

template<typename T, typename scalar>
Vector2<T> operator * (const Vector2<T>& lhs, scalar s)
{
	return Vector2<T>(lhs.x * s, lhs.y * s);
}

template<typename T, typename scalar>
Vector2<T> operator / (const Vector2<T>& lhs, scalar s)
{
	return Vector2<T>(lhs.x / s, lhs.y / s);
}

template<typename T, typename scalar>
Vector2<T> operator * (scalar s, const Vector2<T>& lhs)
{
	return Vector2<T>(lhs.x * s, lhs.y * s);
}


template<typename T>
struct Vector3
{
	union 
	{
		T data[3];
		struct { T x, y, z;};
	};
	T& operator [] (int i)
	{
		return data[i];
	}
	const T& operator [] (int i) const
	{
		return data[i];
	}
	Vector3() : x(0), y(0), z(0) {}
	Vector3(T c) : x(c), y(c), z(c) {}
	Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vector3(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z) {}

	template<typename V>
	static Vector3 MakeVector(const V* Values)
	{
		return Vector3((T)Values[0], (T)Values[1], (T)Values[2]);
	}

	T Dot(const Vector3& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}

	Vector3& operator += (const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3 operator + (const Vector3& rhs) const
	{
		Vector3 Res(*this);
		Res += rhs;
		return Res;
	}

	Vector3 operator * (const Vector3& rhs) const
	{
		return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	Vector3 operator - (const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	T Length() const
	{
		return (T)sqrt(x * x + y * y + z * z);
	}

	Vector3 Normalize() const
	{
		T length = Length();
		T inv = T(1) / length;
		return Vector3(x * inv, y * inv, z *inv);
	}

	Vector3 SafeNormalize(float Epsilon = 1e-4) const
	{
		T length = Length();
		if (length < Epsilon)
		{
			return Vector3(T(0));
		}
		T inv = T(1) / length;
		return Vector3(x * inv, y * inv, z *inv);
	}
};

template<typename T, typename scalar>
Vector3<T> operator * (const Vector3<T>& lhs, scalar s)
{
	return Vector3<T>(lhs.x * s, lhs.y * s, lhs.z * s); 
}

template<typename T, typename scalar>
Vector3<T> operator / (const Vector3<T>& lhs, scalar s)
{
	return Vector3<T>(lhs.x / s, lhs.y / s, lhs.z / s);
}

template<typename T, typename scalar>
Vector3<T> operator * (scalar s, const Vector3<T>& lhs)
{
	return Vector3<T>(lhs.x * s, lhs.y * s, lhs.z * s); 
}

template<typename T>
Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

template<typename T>
Vector3<T> Min(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

template<typename T>
Vector3<T> Max(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
}

template<typename T>
struct Vector4
{
	union 
	{
		T data[4];
		struct { T x, y, z, w;};
	};
	T& operator [] (int i)
	{
		return data[i];
	}
	const T& operator [] (int i) const
	{
		return data[i];
	}
	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(T c) : x(c), y(c), z(c), w(c) {}
	Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	Vector4(const Vector3<T>& v, T s = 0) : x(v.x), y(v.y), z(v.z), w(s) {}

	template<typename V>
	static Vector4 MakeVector(const V* Values)
	{
		return Vector4((T)Values[0], (T)Values[1], (T)Values[2], (T)Values[3]);
	}

	T Dot(const Vector4& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}
};


template<typename T, typename scalar>
Vector4<T> operator * (const Vector4<T>& lhs, scalar s)
{
	return Vector4<T>(lhs.x * s, lhs.y * s, lhs.z * s, lhs.w * s);
}

template<typename T, typename scalar>
Vector4<T> operator * (scalar s, const Vector4<T>& lhs)
{
	return Vector4<T>(lhs.x * s, lhs.y * s, lhs.z * s, lhs.w * s);
}


typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;


struct FBoundingBox
{
	Vector3f BoundMin;
	Vector3f BoundMax;
	FBoundingBox(const Vector3f& BMin = (std::numeric_limits<float>::max)(), const Vector3f& BMax = -(std::numeric_limits<float>::max)())
		: BoundMin(BMin)
		, BoundMax(BMax)
	{}

	void Include(const FBoundingBox& Other);
};


struct FMatrix
{
	union 
	{
		Vector4f row[4];
		struct { Vector4f r0, r1, r2, r3;};
	};
	FMatrix();

	FMatrix(const Vector3f& r0, const Vector3f& r1, const Vector3f& r2, const Vector3f& r3);

	FMatrix(const Vector4f& r0, const Vector4f& r1, const Vector4f& r2, const Vector4f& r3);
	FMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

	Vector4f Column(int i) const;

	FMatrix operator * (const FMatrix& rhs) const;
	FMatrix& operator *= (const FMatrix& rhs);
	FMatrix operator * (float rhs) const;
	FMatrix& operator *= (float rhs);

	Vector4f& operator[](int r) { return row[r]; }
	const Vector4f& operator[](int r) const { return row[r]; }

	Vector3f TranslateVector(const Vector3f& vector) const;
	Vector3f TransformPosition(const Vector3f& position) const;
	FBoundingBox TransformBoundingBox(const FBoundingBox& BoundBox) const;
	FBoundingBox TransformBoundingBox(const Vector3f& BoundMin, const Vector3f& BoundMax) const;
	FMatrix Transpose() const;
	FMatrix Inverse() const;

	static FMatrix TranslateMatrix(const Vector3f& T);
	static FMatrix ScaleMatrix(float s);
	static FMatrix ScaleMatrix(const Vector3f& s);
	static FMatrix RotateX(float v);
	static FMatrix RotateY(float v);
	static FMatrix RotateZ(float v);
	static FMatrix MatrixRotationRollPitchYaw(float Roll, float Pitch, float Yaw);
	static FMatrix MatrixLookAtLH(const Vector3f& EyePosition, const Vector3f& FocusPosition, const Vector3f& UpDirection);
	static FMatrix MatrixPerspectiveFovLH(float FovAngleY, float AspectHByW, float NearZ, float FarZ);
	static FMatrix MatrixOrthoLH(float Width, float Height, float NearZ, float FarZ);
};


struct FQuaternion
{
	Vector4f q;

	FQuaternion(const Vector4f& other) : q(other) {}
	FQuaternion(float x, float y, float z, float w) : q(x, y, z, w) {}
	FQuaternion() {}

	template<typename T>
	static FQuaternion MakeQuaternion(const T* Values)
	{
		return FQuaternion((float)Values[0], (float)Values[1], (float)Values[2], (float)Values[3]);
	}

	static FQuaternion RotationFromAxisAngle(const Vector3f& axis, float angle)
	{
		FQuaternion out{ 0, 0, 0, 1 };
		float      norm = axis.Length();
		if (norm != 0)
		{
			float sina2 = sin(0.5f * angle);
			out.q[0] = sina2 * axis[0] / norm;
			out.q[1] = sina2 * axis[1] / norm;
			out.q[2] = sina2 * axis[2] / norm;
			out.q[3] = cos(0.5f * angle);
		}
		return out;
	}

	void GetAxisAngle(Vector3f& outAxis, float& outAngle) const
	{
		float sina2 = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
		outAngle = 2.0f * atan2(sina2, q[3]);
		float r = (sina2 > 0) ? (1.0f / sina2) : 0;
		outAxis[0] = r * q[0];
		outAxis[1] = r * q[1];
		outAxis[2] = r * q[2];
	}

	static FQuaternion Mul(const FQuaternion& q1, const FQuaternion& q2)
	{
		FQuaternion q1_q2;
		q1_q2.q.x = +q1.q.x * q2.q.w + q1.q.y * q2.q.z - q1.q.z * q2.q.y + q1.q.w * q2.q.x;
		q1_q2.q.y = -q1.q.x * q2.q.z + q1.q.y * q2.q.w + q1.q.z * q2.q.x + q1.q.w * q2.q.y;
		q1_q2.q.z = +q1.q.x * q2.q.y - q1.q.y * q2.q.x + q1.q.z * q2.q.w + q1.q.w * q2.q.z;
		q1_q2.q.w = -q1.q.x * q2.q.x - q1.q.y * q2.q.y - q1.q.z * q2.q.z + q1.q.w * q2.q.w;
		return q1_q2;
	}

	FMatrix ToMatrix() const
	{
		FMatrix out;
		float    yy2 = 2.0f * q[1] * q[1];
		float    xy2 = 2.0f * q[0] * q[1];
		float    xz2 = 2.0f * q[0] * q[2];
		float    yz2 = 2.0f * q[1] * q[2];
		float    zz2 = 2.0f * q[2] * q[2];
		float    wz2 = 2.0f * q[3] * q[2];
		float    wy2 = 2.0f * q[3] * q[1];
		float    wx2 = 2.0f * q[3] * q[0];
		float    xx2 = 2.0f * q[0] * q[0];
		out[0][0] = -yy2 - zz2 + 1.0f;
		out[0][1] = xy2 + wz2;
		out[0][2] = xz2 - wy2;
		out[0][3] = 0;
		out[1][0] = xy2 - wz2;
		out[1][1] = -xx2 - zz2 + 1.0f;
		out[1][2] = yz2 + wx2;
		out[1][3] = 0;
		out[2][0] = xz2 + wy2;
		out[2][1] = yz2 - wx2;
		out[2][2] = -xx2 - yy2 + 1.0f;
		out[2][3] = 0;
		out[3][0] = out[3][1] = out[3][2] = 0;
		out[3][3] = 1;
		return out;
	}
};


//inline Vector4f operator*(const FMatrix& mat, const Vector4f& vec);

inline Vector4f operator*(const Vector4f& vec, const FMatrix& mat);

template <typename T>
T AlignUpWithMask(T Value, size_t Mask)
{
	return (T)(((size_t)Value + Mask) & (~Mask));
}

template <typename T>
T AlignUp(T Value, size_t Alignment)
{
	return AlignUpWithMask(Value, Alignment - 1);
}

template <typename T>
T AlignDownWithMask(T value, size_t mask)
{
	return (T)((size_t)value & ~mask);
}

template <typename T>
T AlignDown(T value, size_t alignment)
{
	return AlignDownWithMask(value, alignment - 1);
}

template <typename T>
bool IsAligned(T value, size_t alignment)
{
	return 0 == ((size_t)value & (alignment - 1));
}


#ifdef _M_X64
#define ENABLE_SSE_CRC32 1
#else
#define ENABLE_SSE_CRC32 0
#endif

#if ENABLE_SSE_CRC32
#pragma intrinsic(_mm_crc32_u32)
#pragma intrinsic(_mm_crc32_u64)
#endif

inline size_t HashRange(const uint32_t* const Begin, const uint32_t* const End, size_t Hash)
{
#if ENABLE_SSE_CRC32
	const uint64_t* Iter64 = (const uint64_t*)AlignUp(Begin, 8);
	const uint64_t* const End64 = (const uint64_t* const)AlignDown(End, 8);

	// If not 64-bit aligned, start with a single u32
	if ((uint32_t*)Iter64 > Begin)
		Hash = _mm_crc32_u32((uint32_t)Hash, *Begin);

	// Iterate over consecutive u64 values
	while (Iter64 < End64)
		Hash = _mm_crc32_u64((uint64_t)Hash, *Iter64++);

	// If there is a 32-bit remainder, accumulate that
	if ((uint32_t*)Iter64 < End)
		Hash = _mm_crc32_u32((uint32_t)Hash, *(uint32_t*)Iter64);
#else
	// An inexpensive hash for CPUs lacking SSE4.2
	for (const uint32_t* Iter = Begin; Iter < End; ++Iter)
		Hash = 16777619U * Hash ^ *Iter;
#endif

	return Hash;
}

template <typename T> inline size_t HashState(const T* StateDesc, size_t Count = 1, size_t Hash = 2166136261U)
{
	static_assert((sizeof(T) & 3) == 0 && alignof(T) >= 4, "State object is not word-aligned");
	return HashRange((uint32_t*)StateDesc, (uint32_t*)(StateDesc + Count), Hash);
}

struct CubeUV
{
	int index;
	float u, v;
};

inline Vector3f CubeUV2XYZ(const CubeUV& c)
{
	float u = c.u * 2.f - 1.f;
	float v = c.v * 2.f - 1.f;
	switch (c.index)
	{
	case 0: return { 1,  v, -u }; 	// +x
	case 1: return { -1,  v,  u }; 	// -x
	case 2: return { u,  1, -v };  // +y
	case 3: return { u, -1,  v };	// -y
	case 4: return { u,  v,  1 };  // +z
	case 5: return { -u,  v, -1 };	// -z
	}
	return Vector3f();
}

inline CubeUV XYZ2CubeUV(const Vector3f& p)
{
	float ax = std::abs(p.x);
	float ay = std::abs(p.y);
	float az = std::abs(p.z);
	CubeUV c;
	if (ax >= ay && ax >= az)	// x face
	{
		c = { p.x >= 0 ? 0 : 1, -p.z / p.x, p.y / ax };
	}
	else if (ay >= az)	// y face
	{
		c = { p.y >= 0 ? 2 : 3, p.x / ay, -p.z / p.y };
	}
	else // z face
	{
		c = { p.z >= 0 ? 4 : 5, p.x / p.z, p.y / az };
	}
	c.u = c.u * 0.5f + 0.5f;
	c.v = c.v * 0.5f + 0.5f;
	return c;
}

inline float NormalRandom(float mu = 0.f, float sigma = 1.f)
{
	unsigned seed = (unsigned)std::chrono::steady_clock::now().time_since_epoch().count();
	static std::default_random_engine generator(seed);
	static std::normal_distribution<float> distribution(mu, sigma);
	return distribution(generator);
}

inline uint32_t ReverseBits(uint32_t Bits)
{
	Bits = (Bits << 16) | (Bits >> 16);
	Bits = ((Bits & 0x00ff00ff) << 8) | ((Bits & 0xff00ff00) >> 8);
	Bits = ((Bits & 0x0f0f0f0f) << 4) | ((Bits & 0xf0f0f0f0) >> 4);
	Bits = ((Bits & 0x33333333) << 2) | ((Bits & 0xcccccccc) >> 2);
	Bits = ((Bits & 0x55555555) << 1) | ((Bits & 0xaaaaaaaa) >> 1);
	return Bits;
}

template <typename T> __forceinline T DivideByMultiple(T value, size_t alignment)
{
	return (T)((value + alignment - 1) / alignment);
}