#pragma once

#include "Global.h"

class CTransform
{
public:
	Vec2 pos{ 0.0,0.0 };
	Vec2 velocity{ 0.0,0.0 };
	float angle{ 0 };

	CTransform(const Vec2& p, const Vec2& s, float a)
		: pos(p),
		velocity(s),
		angle(a) {}
};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setOutlineColor(outline);
		circle.setFillColor(fill);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CScore
{
public:
	int score;
	CScore(int score)
		:score(score) {}
};

class CInput
{
public:
	bool up			{ false };
	bool down		{ false };
	bool left		{ false };
	bool right		{ false };
	bool mouseRight	{ false };
	bool mouseLeft	{ false };

	CInput(){}
};

class CLifeSpan
{
public:
	int remaining	{ 0 }; 
	int total		{ 0 };

	CLifeSpan(int total)		
		: remaining(total), total(total) {}
};

class CCollision
{
public:
	float radius{ 0 };

	CCollision(float r)
		: radius(r) {}
};