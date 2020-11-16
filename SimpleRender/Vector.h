#pragma once
#include "stdfx.h"

template<typename T>
struct Vector2
{
	template<typename U>
	friend std::ostream& operator << (std::ostream& lhs, const Vector2<U>& rhs);

	template<typename U>
	friend std::istream& operator >> (std::istream& lhs, const Vector2<U>& rhs);

	template<typename U>
	friend Vector2<U> operator * (const U& lhs, const Vector2<U>& rhs);

	union 
	{
		struct { T x, y; };
		struct { T u, v; };
	};

	inline Vector2() :x(0), y(0) {};
	inline Vector2(const T& _x) :x(_x), y(_x) {};
	inline Vector2(const T& _x, const T _y) :x(_x), y(_y) {};
	inline Vector2(const Vector2& v) :x(v.x), y(v.y) {};
	inline const Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	inline Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	inline Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	inline Vector2 operator += (const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	inline Vector2 operator -= (const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	inline Vector2 operator + (const T& rhs) const { return Vector2(x + rhs, y + rhs); }
	inline Vector2 operator - (const T& rhs) const { return Vector2(x - rhs, y - rhs); }
	inline Vector2 operator * (const T& rhs) const { return Vector2(x * rhs, y * rhs); }
	inline Vector2 operator / (const T& rhs) const { return Vector2(x / rhs, y / rhs); }
	inline Vector2 operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}
	inline Vector2 operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}
	inline Vector2 operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	inline Vector2 operator /=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	inline bool operator ==(const Vector2& rhs)const { return (x = rhs.x && y == rhs.y); }
	inline bool operator !=(const Vector2& rhs)const { return !(*this == rhs); }
	inline float lengthSqr() { return x * x + y * y; }
	inline float length() { return sqrf(lengthSqr()); }
	inline void normalize() { *this /= length(); }
};

/*-----------------------*/
/* Vector2 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline float Dot(const Vector2<U>& lhs, const Vector2<U>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename U>
inline float Cross(const Vector2<U>& lhs, const Vector2<U>& rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename U>
inline Vector2<U> Normalize(const Vector2<U>& lhs)
{
	float len = lhs.length();
	return lhs / len;
}

template<typename U>
std::ostream& operator << (std::ostream& lhs, const Vector2<U>& rhs)
{
	lhs << rhs.x << " " << rhs.y;
	return lhs;
}

template<typename U>
std::istream& operator >> (std::istream& lhs, const Vector2<U>& rhs)
{
	lhs >> rhs.x >> rhs.y;
	return lhs;
}

template<typename U>
Vector2<U> operator * (const U& lhs, const Vector2<U>& rhs)
{
	return rhs * lhs;
}


/*------*/
/* Vector3 */
/*------*/
template<typename T>
struct Vector3
{
	/*----------------------*/
	/*   friend function   */
	/*---------------------*/
	template<typename U>
	friend std::ostream& operator<<(std::ostream& lhs, const Vector3<U>& rhs);

	template<typename U>
	friend std::istream& operator>>(std::istream& lhs, Vector3<U>& rhs);

	template<typename U>
	friend Vector3<U> operator*(const float& lhs, Vector3<U>& rhs);

	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		T data[3];
	};

	/*--------------*/
	/* Constructors */
	/*--------------*/
	inline Vector3():x(0), y(0), z(0) {};
	inline Vector3(const T& _x) :x(_x), y(_x), z(_x) {};
	inline Vector3(const T& _x, const T& _y, const T& _z) :x(_x), y(_y), z(_z) {};
	//Vector3(const T* t) : x(t[0]), y(t[1]), t(t[2]) {};
	inline Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z) {};
	inline const Vector3& operator = (const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	inline Vector3 operator + (const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3 operator - (const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline const Vector3& operator += (const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline const Vector3& operator -= (const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	inline Vector3 operator + (const T& rhs) const
	{
		return Vector3(x + rhs, y + rhs, z + rhs);
	}

	inline Vector3 operator -(const T& rhs) const
	{
		return Vector3(x - rhs, y - rhs, z - rhs);
	}

	inline Vector3 operator * (const T& rhs) const
	{
		return Vector3(x * rhs, y * rhs, z * rhs);
	}

	inline Vector3 operator / (const T& rhs) const
	{
		return Vector3(x / rhs, y / rhs, z / rhs);
	}

	inline const Vector3& operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	inline const Vector3& operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	inline const Vector3& operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	inline const Vector3& operator /= (const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	inline bool operator == (const Vector3& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline bool operator != (const Vector3& rhs) const
	{
		return !(*this == rhs);
	}

	inline float lengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	inline float length() const
	{
		return sqrt(lengthSqr());
	}

	inline void normalize()
	{
		*this /= length();
	}
};

/*-----------------------*/
/* Vector3 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline float Dot(const Vector3<U>& lhs, const Vector3<U>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename U>
inline Vector3<U> Cross(const Vector3<U>& lhs, const Vector3<U>& rhs)
{
	return Vector3<U>(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}

template<typename U>
inline Vector3<U> Normalize(const Vector3<U>& lhs)
{
	float len = lhs.length();
	return lhs / len;
}

template<typename U>
inline Vector3<U> Mul(const Vector3<U>& lhs, const Vector3<U>& rhs)
{
	return Vector3<U>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const Vector3<T>& rhs)
{
	lhs << rhs.x << " " << rhs.y << " " << rhs.z;
	return lhs;
}

template<typename T>
std::istream& operator>>(std::istream& lhs, Vector3<T>& rhs)
{
	lhs >> rhs.x >> rhs.y >> rhs.z;
	if (!lhs)
	{
		rhs = Vector3<T>();
		std::cout << "Vector3 cin failed" << std::endl;
	}
	return lhs;
}

template<typename U>
inline Vector3<U> operator*(const float& lhs, Vector3<U>& rhs)
{
	return rhs * lhs;
}

/*------*/
/* Vector4 */
/*------*/
template<typename T>
struct Vector4
{
	/*--------------*/
	/*   friend function   */
	/*--------------*/
	template<typename U>
	friend std::ostream& operator<<(std::ostream& lhs, const Vector4<U>& rhs);

	template<typename U>
	friend std::istream& operator>>(std::istream& lhs, Vector4<U>& rhs);

	template<typename U>
	friend Vector4<U> operator*(const U& lhs, Vector4<U>& rhs);

	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T data[4];
	};

	/*--------------*/
	/* Constructors */
	/*--------------*/
	inline Vector4():x(0), y(0), z(0), w(1) {};
	inline Vector4(const T& _x) :x(_x), y(_x), z(_x), w(_x) {};
	inline Vector4(const T& _x, const T& _y, const T& _z, const T& _w = 1) : x(_x), y(_y), z(_z), w(_w) {};
	inline Vector4(const T* t) :x(t[0]), y(t[1]), z(t[2]), w(t[3]) {};
	inline Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
	inline const Vector4& operator = (const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	inline Vector4 operator + (const Vector4& rhs) const
	{
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline Vector4 operator - (const Vector4& rhs) const
	{
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline const Vector4& operator += (const Vector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline const Vector4& operator -= (const Vector4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	inline Vector4 operator + (const T& rhs) const
	{
		return Vector4(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	inline Vector4 operator - (const T& rhs) const
	{
		return Vector4(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	inline Vector4 operator * (const T& rhs) const
	{
		return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	inline Vector4 operator / (const T& rhs) const
	{
		return Vector4(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	inline const Vector4& operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	inline const Vector4& operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	inline const Vector4& operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	inline const Vector4& operator /= (const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	inline bool operator == (const Vector4& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	inline bool operator != (const Vector4& rhs) const
	{
		return !(*this == rhs);
	}

	inline float lengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	inline float length() const
	{
		return sqrt(lengthSqr());
	}

	inline void normalize()
	{
		float len = length();
		x /= len;
		y /= len;
		z /= len;
		w = 0;
	}
};

/*-----------------------*/
/* Vector4 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline float Dot(const Vector4<U>& lhs, const Vector4<U>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<typename U>
inline Vector4<U> Cross(const Vector4<U>& lhs, const Vector4<U>& rhs)
{
	return Vector4<U>(lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x,
		0);
}


template<typename U>
inline Vector4<U> Mul(const Vector4<U>& lhs, const Vector4<U>& rhs)
{
	return Vector4<U>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

template<typename U>
inline Vector4<U> Normalize(const Vector4<U>& rhs)
{
	float len = rhs.length();
	return Vector4<U>(rhs.x /= len, rhs.y /= len, rhs.z /= len, 0);
}

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const Vector4<T>& rhs)
{
	lhs << rhs.x << " " << rhs.y << " " << rhs.z << " " << rhs.w;
	return lhs;
}

template<typename T>
std::istream& operator>>(std::istream& lhs, Vector4<T>& rhs)
{
	lhs >> rhs.x >> rhs.y >> rhs.z >> rhs.w;
	if (!lhs)
	{
		rhs = Vector4<T>();
		std::cout << "Vector3 cin failed" << std::endl;
	}
	return lhs;
}

template<typename U>
inline Vector4<U> operator*(const U& lhs, Vector4<U>& rhs)
{
	return rhs * lhs;
}

using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec3f = Vector3<float>;
using Vec4f = Vector4<float>;