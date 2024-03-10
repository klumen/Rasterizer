#pragma once
#include "Vector.hpp"
#include <cmath>
#include <iostream>
#include <vector>

class Matrix
{
	friend std::ostream& operator<< (std::ostream& cout, const Matrix& m);
	friend Matrix operator* (const float& f, const Matrix& m);
    friend Matrix operator* (const Vector3f& v1, const Vector3f& v2);

public:
	Matrix();
	Matrix(const size_t& initrows, const size_t& initcolumns);
	Matrix(const size_t& initrows, const size_t& initcolumns, const float& initf);
	Matrix(const std::vector<std::vector<float>>& initdata);
	Matrix(const Matrix& m);
	~Matrix();

	void operator= (const std::vector<std::vector<float>>& initdata);
	Matrix operator+ (const Matrix& m) const;
	Matrix operator- (const Matrix& m) const;
	Matrix operator* (const Matrix& m) const;
	Vector4f operator* (const Vector4f& v) const;
	Matrix operator* (const float& f) const;
	Matrix operator/ (const float& f) const;
	Matrix& operator= (const Matrix& m);

	size_t rows, columns;
	std::vector<std::vector<float> > data;

	Matrix& identity();
    Matrix transpose();
	std::vector<std::vector<float>> get_data() const;
	size_t get_rows() const;
	size_t get_columns() const;

private:

};

std::ostream& operator<< (std::ostream& cout, const Matrix& m);

Matrix operator* (const float& f, const Matrix& m);

Matrix operator* (const Vector3f& v1, const Vector3f& v2);