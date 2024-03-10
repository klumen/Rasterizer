#pragma once
#include <iostream>
#include <cmath>
#include <vector>

class Vector4f;

class Vector
{
	friend Vector operator* (const float& f, const Vector& v);

public:
	Vector();
	Vector(const size_t initrows);
	Vector(const std::vector<float>& initdata);
	Vector(const Vector &v);
	~Vector();

	void operator= (const std::vector<float>& initdata);
	Vector operator+ (const Vector& v);
	Vector operator- (const Vector& v);
	Vector operator* (const float& f);
	Vector operator/ (const float& f);
	Vector& operator= (const Vector& v);

	size_t size() const;
	std::vector <float> get_data() const;
	float dot(const Vector& v) const;
	float norm();
	Vector normalized();

private:
	size_t rows;
	std::vector <float> data;

};

std::ostream& operator<< (std::ostream& cout, const Vector& v);

class Vector2f
{
public:
	Vector2f();
	Vector2f(const float& xx);
	Vector2f(const float& xx, const float& yy);
	Vector2f(const Vector2f& v);
	~Vector2f();

	Vector2f operator+ (const Vector2f& v) const;
	Vector2f operator- (const Vector2f& v) const;
	Vector2f operator* (const float& f) const;
	Vector2f operator/ (const float& f) const;
	Vector2f& operator= (const Vector2f& v);

	float dot(const Vector2f& v) const;
	float norm() const;
	Vector2f normalized() const;

	float x, y;

private:

};

std::ostream& operator<< (std::ostream& cout, const Vector2f& v);

Vector2f operator* (const float& f, const Vector2f& v);

class Vector3f
{
    friend Vector3f operator* (const float& f, const Vector3f& v);
    friend std::ostream& operator<< (std::ostream& cout, const Vector3f& v);

public:
	Vector3f();
	Vector3f(const float& xx);
	Vector3f(const float& xx, const float& yy, const float& zz);
	Vector3f(const Vector3f& v);
	~Vector3f();
	
	Vector3f operator+ (const Vector3f& v) const;
	Vector3f operator- (const Vector3f& v) const;
	Vector3f operator* (const float& f) const;
	Vector3f operator/ (const float& f) const;
	Vector3f& operator= (const Vector3f& v);

	float dot(const Vector3f& v) const;
	Vector3f cross(const Vector3f& v) const;
    Vector3f cwise(const Vector3f& v) const;
	float norm() const;
	Vector3f normalized() const;
	Vector4f to_Vector4f(const float& w) const;

	float x, y, z;

private:

};

std::ostream& operator<< (std::ostream& cout, const Vector3f& v);

Vector3f operator* (const float& f, const Vector3f& v);

class Vector3i
{
public:
	Vector3i();
	Vector3i(const size_t& xx);
	Vector3i(const size_t& xx, const size_t& yy, const size_t& zz);
	Vector3i(const Vector3i& v);
	~Vector3i();

	Vector3i operator+ (const Vector3i& v);
	Vector3i operator- (const Vector3i& v);
	Vector3i operator* (const size_t& f);
	Vector3i operator/ (const size_t& f);
	Vector3i& operator= (const Vector3i& v);

	size_t x, y, z;

private:

};

std::ostream& operator<< (std::ostream& cout, const Vector3i& v);

class Vector4f
{
public:
	Vector4f();
	Vector4f(const float& xx);
	Vector4f(const float& xx, const float& yy, const float& zz, const float& ww);
	Vector4f(const Vector4f& v);
	~Vector4f();

	Vector4f operator+ (const Vector4f& v);
	Vector4f operator- (const Vector4f& v);
	Vector4f operator* (const float& f);
	Vector4f operator/ (const float& f);
	Vector4f& operator= (const Vector4f& v);

	float dot(const Vector4f& v) const;
	Vector4f cross(const Vector4f& v);
	float norm();
	Vector4f normalized();
	Vector3f to_Vector3f() const;

	float x, y, z, w;

private:

};

std::ostream& operator<< (std::ostream& cout, const Vector4f& v);