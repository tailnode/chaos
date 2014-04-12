#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <math.h>

// 3D vector
class Vector3D {
	public:
		Vector3D(double x, double y, double z):m_x(x),m_y(y),m_z(z){}

		inline void x(double x);
		inline void y(double y);
		inline void z(double z);
		inline double x() const;
		inline double y() const;
		inline double z() const;
		inline void toZeroVector();
		inline Vector3D getNegtive() const;
		inline double calcModel() const;
		inline Vector3D normalize() const;
		inline double distance(const Vector3D& v) const;
		inline bool operator==(const Vector3D& v) const;
		inline Vector3D operator*(double s) const;
		inline Vector3D operator/(double s) const;
		inline Vector3D operator+(const Vector3D& v) const;
		inline Vector3D operator-(const Vector3D& v) const;
		inline double dotProduct(const Vector3D& v) const;
		inline Vector3D crossProduct(const Vector3D& v) const;
				
	private:
		double m_x;
		double m_y;
		double m_z;
};

inline const Vector3D operator*(double s, const Vector3D& v)
{
	return v * s;
}

inline void Vector3D::x(double x)
{
    m_x = x;
}

inline void Vector3D::y(double y)
{
    m_y = y;
}

inline void Vector3D::z(double z)
{
    m_z = z;
}

inline double Vector3D::x() const 
{
    return m_x;
}

inline double Vector3D::y() const 
{
    return m_y;
}

inline double Vector3D::z() const 
{
    return m_z;
}

inline void Vector3D::toZeroVector()
{
    m_x = m_y = m_z = 0;
}

inline Vector3D Vector3D::getNegtive() const
{
    return Vector3D(-m_x, -m_y, -m_z);
}

inline double Vector3D::calcModel() const 
{
    return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

inline Vector3D Vector3D::normalize() const
{
	double m = calcModel();
	return Vector3D(m_x / m, m_y / m, m_z / m);
}

inline double Vector3D::distance(const Vector3D& v) const
{
	return sqrt((m_x - v.m_x) * (m_x - v.m_x) +
				(m_y - v.m_y) * (m_y - v.m_y) +
				(m_z - v.m_z) * (m_z - v.m_z));
}

inline bool Vector3D::operator==(const Vector3D& v) const
{
	return (m_x == v.m_x &&
			m_y == v.m_y &&
			m_z == v.m_z);
}

inline Vector3D Vector3D::operator*(double s) const
{
	return Vector3D(s * m_x, s * m_y, s * m_z);
}

inline Vector3D Vector3D::operator/(double s) const
{
	return Vector3D(m_x / s, m_y / s, m_z / s);
}

inline Vector3D Vector3D::operator+(const Vector3D& v) const
{
	return Vector3D(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

inline Vector3D Vector3D::operator-(const Vector3D& v) const
{
	return Vector3D(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

inline double Vector3D::dotProduct(const Vector3D& v) const
{
	return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

inline Vector3D Vector3D::crossProduct(const Vector3D& v) const
{
	double x = m_y * v.m_z - m_z * v.m_y;
	double y = m_z * v.m_x - m_x * v.m_z;
	double z = m_x * v.m_y - m_y * v.m_x;

	return Vector3D(x, y, z);
}

#endif // _VECTOR3D_H_
