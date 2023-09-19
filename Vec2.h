#pragma once

class Vec2
{

public:

	double x = 0;
	double y = 0;

	Vec2();
	Vec2(double xin, double yin);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const double val) const;
	Vec2 operator * (const double val) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator /= (const double val);
	void operator *= (const double val);

	void normalise();
	static Vec2 normalise(const Vec2 vec);

	double length();

};
