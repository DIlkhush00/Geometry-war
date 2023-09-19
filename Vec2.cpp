#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{

}

Vec2::Vec2(double xin, double yin)
	: x(xin), y(yin)
{

}

bool Vec2::operator == (const Vec2& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return (x != rhs.x) && (y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const double val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const double val) const
{
	return Vec2(x * val, y * val);
}

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator /= (const double val)
{
	x /= val;
	y /= val;
}

void Vec2::operator *= (const double val)
{
	x *= val;
	y *= val;
}

void Vec2::normalise()
{
	double L{ sqrt(x * x + y * y) };
	x = x / L;
	y = y / L;
}

Vec2 Vec2::normalise(Vec2 vector)
{
	double L{ sqrt(vector.x * vector.x + vector.y * vector.y) };
	return Vec2(vector.x / L, vector.x / L);
}

double Vec2::length()
{
	return sqrt(x * x + y * y);
}