#pragma once
#include <string>
#include <cmath>

struct Vector2 {
	float X;
	float Y;

	Vector2(float x, float y): X(x), Y(y) { }
	Vector2(): X(0.0f), Y(0.0f) { }
	Vector2(float value): X(value), Y(value) { }
	Vector2(const Vector2* vector): X(vector->X), Y(vector->Y) { }

	std::string ToString() {
		return std::to_string(X) + ", " + std::to_string(Y);
	}

	Vector2& Normalize();
	static float Distance(const Vector2& v1, const Vector2& v2);
	static Vector2 Direction(const Vector2& to, const Vector2& from);
	static float DotProduct(const Vector2& v1, const Vector2& v2);
	static float ToRadians(const Vector2& v1, const Vector2& v2);
	Vector2 Normalized();
	float Magnitude();

	friend Vector2 operator+(Vector2& v1, const Vector2& v2);
	friend Vector2 operator-(Vector2& v1, const Vector2& v2);
	friend Vector2 operator*(Vector2& v1, const Vector2& v2);
	friend Vector2 operator/(Vector2& v1, const Vector2& v2);

	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator/=(const Vector2& vec);

	//inline static Vector2 Forward() {
	//	return Vector2 {1, 0};
	//}


private:
	Vector2& Add(const Vector2& vec);
	Vector2& Subtract(const Vector2& vec);
	Vector2& Multiply(const Vector2& vec);
	Vector2& Divide(const Vector2& vec);

};

