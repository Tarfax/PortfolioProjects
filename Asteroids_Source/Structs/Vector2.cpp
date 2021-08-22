#include "Vector2.h"
#include "Math/Mathf.h"

Vector2& Vector2::Add(const Vector2& vec)
{
	this->X += vec.X;
	this->Y += vec.Y;

	return *this;
}

Vector2& Vector2::Subtract(const Vector2& vec)
{
	this->X -= vec.X;
	this->Y -= vec.Y;

	return *this;
}

Vector2& Vector2::Multiply(const Vector2& vec)
{
	this->X *= vec.X;
	this->Y *= vec.Y;

	return *this;
}

Vector2& Vector2::Divide(const Vector2& vec)
{
	this->X /= vec.X;
	this->Y /= vec.Y;

	return *this;
}

Vector2& Vector2::Normalize()
{
	float length = std::sqrt(X * X + Y * Y);
	X /= length;
	Y /= length;
	return *this;
}

Vector2 Vector2::Normalized() {
	if (X != 0 || Y != 0) {
		float length = Magnitude();
		return Vector2(
			X / length,
			Y / length);
	}
	else {
		return Vector2(0, 0);
	}
}

/// <summary>
/// The length of this vector
/// </summary>
/// <returns></returns>
float Vector2::Magnitude()
{
	return sqrt((X * X) + (Y * Y));
}

float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
	float distance = ((v1.X - v2.X) * (v1.X - v2.X) + (v1.Y - v2.Y) * (v1.Y - v2.Y));
	return distance;
}

Vector2 Vector2::Direction(const Vector2& to, const Vector2& from) {
	Vector2 diff;

	diff.X = to.X - from.X;
	diff.Y = to.Y - from.Y;

	return diff.Normalized();
}

float Vector2::DotProduct(const Vector2& v1, const Vector2& v2) {
	return v1.X * v2.X + v1.Y * v2.Y;
}

float Vector2::ToRadians(const Vector2& v1, const Vector2& v2)
{
	Vector2 diff;

	diff.X = ((v1.X > v2.X) ? v1.X - v2.X : v2.X - v1.X);
	diff.Y = ((v1.Y > v2.Y) ? v1.Y - v2.Y : v2.Y - v1.Y);

	return atan(diff.Y / diff.X) * 180.0f / PI;
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	return this->Add(vec);
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	return this->Subtract(vec);
}

Vector2& Vector2::operator*=(const Vector2& vec)
{
	return this->Multiply(vec);
}

Vector2& Vector2::operator/=(const Vector2& vec)
{
	return this->Divide(vec);
}

Vector2 operator+(Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.X + v2.X, v1.Y + v2.Y);
}

Vector2 operator-(Vector2& v1, const Vector2& v2)
{
	return  Vector2(v1.X - v2.X, v1.Y - v2.Y);
}

Vector2 operator*(Vector2& v1, const Vector2& v2)
{
	return  Vector2(v1.X * v2.X, v1.Y * v2.Y);
}

Vector2 operator/(Vector2& v1, const Vector2& v2)
{
	return  Vector2(v1.X / v2.X, v1.Y / v2.Y);
}