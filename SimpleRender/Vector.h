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

	Vector2() {};
	Vector2(const T& _x) :x(_x), y(_x) {};
	Vector2(const T& _x, const T _y) :x(_x), y(_y) {};
	Vector2(const Vector2& v) :x(v.x), y(v.y) {};
	const Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator += (const Vector2& rhs) 
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2 operator -= (const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2 operator + (const T& rhs) const { return Vector2(x + rhs, y + rhs); }
	Vector2 operator - (const T& rhs) const { return Vector2(x - rhs, y - rhs); }
	Vector2 operator * (const T& rhs) const { return Vector2(x * rhs, y * rhs); }
	Vector2 operator / (const T& rhs) const { return Vector2(x / rhs, y / rhs); }
	Vector2 operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}
	Vector2 operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}
	Vector2 operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vector2 operator /=(const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	bool operator ==(const Vector2& rhs)const { return (x = rhs.x && y == rhs.y); }
	bool operator !=(const Vector2& rhs)const { return !(*this == rhs); }
	T lengthSqr() { return x * x + y * y; }
	T length() { return sqrt(lengthSqr()); }
	void normalize() { *this /= length(); }
};

/*-----------------------*/
/* Vector2 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vector2<U>& lhs, const Vector2<U>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template<typename U>
inline U Cross(const Vector2<U>& lhs, const Vector2<U>& rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

template<typename U>
inline Vector2<U> Normalize(const Vector2<U>& lhs)
{
	U len = lhs.Length();
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
	Vector3() {};
	Vector3(const T& _x) :x(_x), y(_x), z(_x) {};
	Vector3(const T& _x, const T& _y, const T& _z) :x(_x), y(_y), z(_z) {};
	Vector3(const T* t) : x(t[0]), y(t[1]), t(t[2]) {};
	Vector3(const Vector3& v) :x(v.x), y(v.y), z(v.z) {};
	const Vector3& operator = (const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Vector Math */
	/*-------------------*/
	Vector3 operator + (const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3 operator - (const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	const Vector3& operator += (const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	const Vector3& operator -= (const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/*-------------------*/
	/* Basic Scalar Math */
	/*-------------------*/
	Vector3 operator + (const T& rhs) const
	{
		return Vector3(x + rhs, y + rhs, z + rhs);
	}

	Vector3 operator -(const T& rhs) const
	{
		return Vector3(x - rhs, y - rhs, z - rhs);
	}

	Vector3 operator * (const T& rhs) const
	{
		return Vector3(x * rhs, y * rhs, z * rhs);
	}

	Vector3 operator / (const T& rhs) const
	{
		return Vector3(x / rhs, y / rhs, z / rhs);
	}

	const Vector3& operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	const Vector3& operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	const Vector3& operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	const Vector3& operator /= (const T& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/*------------*/
	/* Other Math */
	/*------------*/
	bool operator == (const Vector3& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	bool operator != (const Vector3& rhs) const
	{
		return !(*this == rhs);
	}

	T lengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	T length() const
	{
		return sqrt(lengthSqr());
	}

	void normalize()
	{
		*this /= length();
	}
};

/*-----------------------*/
/* Vector3 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vector3<U>& lhs, const Vector3<U>& rhs)
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
	U len = lhs.length();
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
Vector3<U> operator*(const float& lhs, Vector3<U>& rhs)
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
	Vector4() {};
	Vector4(const T& _x) :x(_x), y(_x), z(_x), w(_x) {};
	Vector4(const T& _x, const T& _y, const T& _z, const T& _w = 1) : x(_x), y(_y), z(_z), w(_w) {};
	Vector4(const T* t) :x(t[0]), y(t[1]), z(t[2]), w(t[3]) {};
	Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {};
	const Vector4& operator = (const Vector4& v)
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
	Vector4 operator + (const Vector4& rhs) const
	{
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vector4 operator - (const Vector4& rhs) const
	{
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	const Vector4& operator += (const Vector4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	const Vector4& operator -= (const Vector4& rhs)
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
	Vector4 operator + (const T& rhs) const
	{
		return Vector4(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	Vector4 operator - (const T& rhs) const
	{
		return Vector4(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	Vector4 operator * (const T& rhs) const
	{
		return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	Vector4 operator / (const T& rhs) const
	{
		return Vector4(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	const Vector4& operator += (const T& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;
		return *this;
	}

	const Vector4& operator -= (const T& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;
		return *this;
	}

	const Vector4& operator *= (const T& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	const Vector4& operator /= (const T& rhs)
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
	bool operator == (const Vector4& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	bool operator != (const Vector4& rhs) const
	{
		return !(*this == rhs);
	}

	T lengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	T length() const
	{
		return sqrt(lengthSqr());
	}

	void normalize()
	{
		*this /= length();
	}

	void homogeneous()
	{
		x /= w;
		y /= w;
		z /= w;
		w = 1;
	}
};

/*-----------------------*/
/* Vector4 Nomenberfunction */
/*-----------------------*/
template<typename U>
inline U Dot(const Vector4<U>& lhs, const Vector4<U>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template<typename U>
Vector4<U> Cross(const Vector4<U>& lhs, const Vector4<U>& rhs)
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
	U len = rhs.length();
	return rhs / len;
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
Vector4<U> operator*(const U& lhs, Vector4<U>& rhs)
{
	return rhs * lhs;
}

using Vec2i = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec3f = Vector3<float>;
using Vec4f = Vector4<float>;