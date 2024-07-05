#pragma once
#include <cmath>
#include <numbers>

template< typename T >
struct matrix_t
{
	T data[16];
};

template< typename T >
struct vector3_t
{
	T x, y, z;
};

template< typename T >
class Quat
{
	T m_value[4];

  public:
	const T *data() const { return m_value; }

	Quat(T x = 0, T y = 0, T z = 0, T w = 0) : m_value{ y, z, w, x } {}

	Quat(T corner, bool format, vector3_t< T > imaginary)
	{
		T norm = std::sqrt(imaginary.x * imaginary.x + imaginary.y * imaginary.y + imaginary.z * imaginary.z);
		corner = (format ? corner : (corner * (T) std::numbers::pi) / 180) / 2;
		m_value[3] = std::cos(corner);
		m_value[0] = imaginary.x / norm * std::sin(corner);
		m_value[1] = imaginary.y / norm * std::sin(corner);
		m_value[2] = imaginary.z / norm * std::sin(corner);
	}

	Quat operator+(Quat const &x) const
	{
		Quat y = *this;
		y += x;
		return y;
	}

	Quat &operator+=(Quat const &x)
	{
		this->m_value[3] += x.m_value[3], this->m_value[0] += x.m_value[0], this->m_value[1] += x.m_value[1],
			this->m_value[2] += x.m_value[2];
		return *this;
	}

	Quat operator-(Quat const &x) const
	{
		Quat y = *this;
		y -= x;
		return y;
	}

	Quat &operator-=(Quat const &x)
	{
		this->m_value[3] -= x.m_value[3], this->m_value[0] -= x.m_value[0], this->m_value[1] -= x.m_value[1],
			this->m_value[2] -= x.m_value[2];
		return *this;
	}

	Quat operator*(Quat const &x) const
	{
		return {
			this->m_value[3] * x.m_value[3] - this->m_value[0] * x.m_value[0] - this->m_value[1] * x.m_value[1] -
				this->m_value[2] * x.m_value[2],
			this->m_value[3] * x.m_value[0] + this->m_value[0] * x.m_value[3] + this->m_value[1] * x.m_value[2] -
				this->m_value[2] * x.m_value[1],
			this->m_value[3] * x.m_value[1] - this->m_value[0] * x.m_value[2] + this->m_value[1] * x.m_value[3] +
				this->m_value[2] * x.m_value[0],
			this->m_value[3] * x.m_value[2] + this->m_value[0] * x.m_value[1] - this->m_value[1] * x.m_value[0] +
				this->m_value[2] * x.m_value[3]
		};
	}

	Quat operator*(vector3_t< T > const &v) const
	{
		Quat f{ 0, v.x, v.y, v.z };
		return *this * f;
	}

	Quat operator*(const T x) const
	{
		return { this->m_value[3] * x, this->m_value[0] * x, this->m_value[1] * x, this->m_value[2] * x };
	}

	Quat operator~() const { return { m_value[3], -m_value[0], -m_value[1], -m_value[2] }; }

	bool operator==(Quat const &x) const
	{
		return this->m_value[3] == x.m_value[3] && this->m_value[0] == x.m_value[0] &&
			   this->m_value[1] == x.m_value[1] && this->m_value[2] == x.m_value[2];
	}

	bool operator!=(Quat const &x) const { return !(*this == x); }

	explicit operator T() const
	{
		return std::sqrt(m_value[3] * m_value[3] + m_value[0] * m_value[0] + m_value[1] * m_value[1] + m_value[2] * m_value[2]);
	}

	matrix_t< T > matrix() const
	{
		return { m_value[3], -m_value[0], -m_value[1], -m_value[2], m_value[0], m_value[3],	 -m_value[2], m_value[1],
				 m_value[1], m_value[2],  m_value[3],  -m_value[0], m_value[2], -m_value[1], m_value[0],  m_value[3] };
	}

	T angle(const bool format = true) const
	{
		return (format ? std::acos(m_value[3] / T(*this)) : std::acos(m_value[3] / T(*this)) * 180 / std::numbers::pi) * 2;
	}

	vector3_t< T > apply(vector3_t< T > x) const
	{
		T norm = T(*this);
		Quat y{ m_value[3] / norm, m_value[0] / norm, m_value[1] / norm, m_value[2] / norm };
		Quat z = y * x * ~y;
		vector3_t< T > result{ z.m_value[0], z.m_value[1], z.m_value[2] };
		return result;
	}

	matrix_t< T > rotation_matrix() const
	{
		T norm = T(*this);
		T x = m_value[3] / norm, y = m_value[0] / norm, z = m_value[1] / norm, w = m_value[2] / norm;
		return {
			1 - 2 * z * z - 2 * w * w,
			2 * y * z + 2 * w * x,
			2 * y * w - 2 * z * x,
			0,
			2 * y * z - 2 * w * x,
			1 - 2 * y * y - 2 * w * w,
			2 * z * w + 2 * y * x,
			0,
			2 * y * w + 2 * z * x,
			2 * z * w - 2 * y * x,
			1 - 2 * y * y - 2 * z * z,
			0,
			0,
			0,
			0,
			1
		};
	}
};
