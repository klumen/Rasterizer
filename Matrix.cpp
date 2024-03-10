#include "Matrix.hpp"

Matrix::Matrix() : rows(0), columns(0) {}

Matrix::Matrix(const size_t& initrows, const size_t& initcolumns) : rows(initrows), columns(initcolumns)
{
	data.resize(rows);
	for (auto& i : data)
		i.resize(columns);
}

Matrix::Matrix(const size_t& initrows, const size_t& initcolumns, const float& initf) : rows(initrows), columns(initcolumns)
{
	data.resize(rows);
	for (auto& i : data)
		i.resize(columns);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data[i][j] = initf;
}

Matrix::Matrix(const std::vector<std::vector<float>>& initdata) : data(initdata)
{
	rows = data.size();
	rows == 0 ? columns = 0 : columns = data[0].size();
}

Matrix::Matrix(const Matrix& m) : data(m.data)
{
	rows = m.rows;
	columns = m.columns;
}

Matrix::~Matrix() {}

void Matrix::operator= (const std::vector<std::vector<float>>& initdata)
{
	data = initdata;
}

Matrix Matrix::operator+ (const Matrix& m) const
{
	auto data1 = get_data();
	auto data2 = m.get_data();

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data1[i][j] += data2[i][j];

	return Matrix(data1);
}

Matrix Matrix::operator- (const Matrix& m) const
{
	auto data1 = get_data();
	auto data2 = m.get_data();

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data1[i][j] -= data2[i][j];

	return Matrix(data1);
}

Matrix Matrix::operator* (const Matrix& m) const
{
	auto data1 = get_data();
	auto data2 = m.get_data();
	std::vector<std::vector<float>> data(rows, std::vector<float>(m.columns));

	for (int i = 0; i < rows; i++)
		for (int k = 0; k < columns; k++)
			for (int j = 0; j < columns; j++)
					data[i][j] += data1[i][k] * data2[k][j];

	return Matrix(data);
}

Vector4f Matrix::operator* (const Vector4f& v) const
{
	auto data1 = get_data();
	std::vector<float> data(4);

	for (int i = 0; i < rows; i++)
			data[i] = data1[i][0] * v.x + data1[i][1] * v.y + data1[i][2] * v.z + data1[i][3] * v.w;

	return Vector4f(data[0], data[1], data[2], data[3]);
}

Matrix operator* (const Vector3f& v1, const Vector3f& v2)
{
    Matrix m(3, 3);

    m.data[0][0] = v1.x * v2.x;
    m.data[0][1] = v1.x * v2.y;
    m.data[0][2] = v1.x * v2.z;
    m.data[1][0] = v1.y * v2.x;
    m.data[1][1] = v1.y * v2.y;
    m.data[1][2] = v1.y * v2.z;
    m.data[2][0] = v1.z * v2.x;
    m.data[2][1] = v1.z * v2.y;
    m.data[2][2] = v1.z * v2.z;

    return m;
}

Matrix Matrix::operator* (const float& f) const
{
	auto data1 = get_data();
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data1[i][j] *= f;

	return Matrix(data1);
}

Matrix operator* (const float& f, const Matrix& m)
{
	auto data1 = m.get_data();

	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.columns; j++)
			data1[i][j] *= f;

	return Matrix(data1);
}

Matrix Matrix::operator/ (const float& f) const
{
	auto data1 = get_data();

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data1[i][j] /= f;

	return Matrix(data1);
}

Matrix& Matrix::operator= (const Matrix& m)
{
	if (this == &m)
		return *this;

	data = m.data;
    columns = m.columns;
    rows = m.rows;

	return *this;
}

std::ostream& operator<< (std::ostream& cout, const Matrix& m)
{
	cout << "Matrix(" << m.rows << ", " << m.columns << ")" << std::endl;

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.columns; j++)
		{
			cout << m.data[i][j];
			if (j < m.columns - 1)
				cout << ", ";
		}
		if(i < m.rows - 1)
			cout << std::endl;
	}
		
	return cout;
}

Matrix& Matrix::identity()
{
	for (int i = 0; i < rows; i++)
		data[i][i] = 1.0f;
	return *this;
}

Matrix Matrix::transpose()
{
    Matrix m(columns, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            m.data[j][i] = data[i][j];
    return m;
}

std::vector<std::vector<float>> Matrix::get_data() const { return data; }

size_t Matrix::get_rows() const { return rows; }

size_t Matrix::get_columns() const { return columns; }
