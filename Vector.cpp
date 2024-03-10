#include "Vector.hpp"

Vector::Vector() : rows(0) {}

Vector::Vector(const size_t initrows) : rows(initrows) { data.resize(rows); }

Vector::Vector(const std::vector<float>& initdata) : data(initdata) { rows = data.size(); }

Vector::Vector(const Vector& v) : rows(v.rows), data(v.get_data()) {}

Vector::~Vector() {}

void Vector::operator= (const std::vector<float>& initdata) { data = initdata; }

Vector Vector::operator+ (const Vector& v)
{ 
	std::vector<float> data1 = data;
	auto data2 = v.get_data();
	for (int i = 0; i < rows; i++)
		data1[i] += data2[i];
	return Vector(data1);
}

Vector Vector::operator- (const Vector& v) 
{
	std::vector<float> data1 = data;
	auto data2 = v.get_data();
	for (int i = 0; i < rows; i++)
		data1[i] -= data2[i];
	return Vector(data1);
}

Vector Vector::operator* (const float& f)
{
	std::vector<float> data1 = data;
	for (int i = 0; i < rows; i++)
		data1[i] *= f;
	return Vector(data1);
}

Vector operator* (const float& f, const Vector& v)
{
	std::vector<float> data1 = v.data;
	for (int i = 0; i < v.rows; i++)
		data1[i] *= f;
	return Vector(data1);
}

Vector Vector::operator/ (const float& f)
{
	std::vector<float> data1 = data;
	for (int i = 0; i < rows; i++)
		data1[i] /= f;
	return Vector(data1);
}

Vector& Vector::operator= (const Vector& v)
{
	if (&v == this)
		return *this;
	data = v.get_data();
	rows = v.size();
	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Vector& v)
{
	cout << "Vector"  << v.size() << "f(";
	for (int i = 0; i < v.size(); i++)
	{
		cout << v.get_data()[i];
		if (i != v.size() - 1)
			cout << ", ";
	}
	cout << ")";
	return cout;
}

size_t Vector::size() const { return rows; }

std::vector <float> Vector::get_data() const { return data; }

float Vector::dot(const Vector& v) const
{
	float sum = 0.0f;
	std::vector<float> data1 = data;
	auto data2 = v.get_data();
	for (int i = 0; i < rows; i++)
		sum += data1[i] * data2[i];
	return sum;
}

float Vector::norm()
{
	float sum = 0.0f;
	for (auto i : data)
		sum += i * i;
	return sqrt(sum);
}

Vector Vector::normalized()
{
	return *this / this->norm();
}

Vector2f::Vector2f() : x(0.0f), y(0.0f) {}

Vector2f::Vector2f(const float& xx) : x(xx), y(xx) {}

Vector2f::Vector2f(const float& xx, const float& yy) : x(xx), y(yy) {}

Vector2f::Vector2f(const Vector2f& v) : x(v.x), y(v.y) {}

Vector2f::~Vector2f() {}

Vector2f Vector2f::operator+ (const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }

Vector2f Vector2f::operator- (const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }

Vector2f Vector2f::operator* (const float& f) const { return Vector2f(x * f, y * f); }

Vector2f operator* (const float& f, const Vector2f& v) { return Vector2f(f * v.x, f * v.y); }

Vector2f Vector2f::operator/ (const float& f) const { return Vector2f(x / f, y / f); }

// 返回引用
Vector2f& Vector2f::operator= (const Vector2f& v)
{
	if (this != &v) // 防止自身赋值
		x = v.x, y = v.y;
	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Vector2f& v)
{
	cout << "Vector2f(" << v.x << ", " << v.y << ")";
	return cout;
}

float Vector2f::dot(const Vector2f& v) const
{
	return x * v.x + y * v.y;
}

float Vector2f::norm() const
{
	return std::sqrt(x * x + y * y);
}

Vector2f Vector2f::normalized() const
{
	return *this / this->norm();
}

Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3f::Vector3f(const float& xx) : x(xx), y(xx), z(xx) {}

Vector3f::Vector3f(const float& xx, const float& yy, const float& zz) : x(xx), y(yy), z(zz) {}

Vector3f::Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

Vector3f::~Vector3f() {}

Vector3f Vector3f::operator+ (const Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }

Vector3f Vector3f::operator- (const Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }

Vector3f Vector3f::operator* (const float& f) const { return Vector3f(x * f, y * f, z * f); }

Vector3f operator* (const float& f, const Vector3f& v) { return Vector3f(f * v.x, f * v.y, f * v.z); }

Vector3f Vector3f::operator/ (const float& f) const
{
	return Vector3f(x / f, y / f, z / f);
}

// 返回引用
Vector3f& Vector3f::operator= (const Vector3f& v)
{
	if (this != &v) // 防止自身赋值
		x = v.x, y = v.y, z = v.z;
	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Vector3f& v)
{
	cout << "Vector3f(" << v.x << ", " << v.y << ", " << v.z << ")";
	return cout;
}

float Vector3f::dot(const Vector3f& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3f Vector3f::cross(const Vector3f& v) const
{
	return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

Vector3f Vector3f::cwise(const Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }

float Vector3f::norm() const
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector3f Vector3f::normalized() const
{
	return *this / this->norm();
}

Vector4f Vector3f::to_Vector4f(const float& w) const { return Vector4f(x, y, z, w); }

Vector3i::Vector3i() : x(0), y(0), z(0) {}

Vector3i::Vector3i(const size_t& xx) : x(xx), y(xx), z(xx) {}

Vector3i::Vector3i(const size_t& xx, const size_t& yy, const size_t& zz) : x(xx), y(yy), z(zz) {}

Vector3i::Vector3i(const Vector3i& v) : x(v.x), y(v.y), z(v.z) {}

Vector3i::~Vector3i() {}

Vector3i Vector3i::operator+ (const Vector3i& v)
{
	return Vector3i(x + v.x, y + v.y, z + v.z);
}

Vector3i Vector3i::operator- (const Vector3i& v)
{
	return Vector3i(x - v.x, y - v.y, z - v.z);
}

Vector3i Vector3i::operator* (const size_t& f)
{
	return Vector3i(x * f, y * f, z * f);
}

Vector3i Vector3i::operator/ (const size_t& f)
{
	return Vector3i(x / f, y / f, z / f);
}

// 返回引用
Vector3i& Vector3i::operator= (const Vector3i& v)
{
	if (this != &v) // 防止自身赋值
		x = v.x, y = v.y, z = v.z;
	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Vector3i& v)
{
	cout << "Vector3i(" << v.x << ", " << v.y << ", " << v.z << ")";
	return cout;
}

Vector4f::Vector4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vector4f::Vector4f(const float& xx) : x(xx), y(xx), z(xx), w(xx) {}

Vector4f::Vector4f(const float& xx, const float& yy, const float& zz, const float& ww) : x(xx), y(yy), z(zz), w(ww) {}

Vector4f::Vector4f(const Vector4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

Vector4f::~Vector4f() {}

Vector4f Vector4f::operator+ (const Vector4f& v)
{
	return Vector4f(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4f Vector4f::operator- (const Vector4f& v)
{
	return Vector4f(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4f Vector4f::operator* (const float& f)
{
	return Vector4f(x * f, y * f, z * f, w * f);
}

Vector4f Vector4f::operator/ (const float& f)
{
	return Vector4f(x / f, y / f, z / f, w / f);
}

// 返回引用
Vector4f& Vector4f::operator= (const Vector4f& v)
{
	if (this != &v) // 防止自身赋值
		x = v.x, y = v.y, z = v.z, w = v.w;
	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Vector4f& v)
{
	cout << "Vector4f(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return cout;
}

float Vector4f::dot(const Vector4f& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector4f Vector4f::cross(const Vector4f& v)
{
	return Vector4f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, v.w);
}

float Vector4f::norm()
{
	return std::sqrt(x * x + y * y + z * z);
}

Vector4f Vector4f::normalized()
{
	return *this / this->norm();
}

Vector3f Vector4f::to_Vector3f() const { return w == 0.0f ? Vector3f(x, y, z) : Vector3f(x / w, y / w, z / w); }