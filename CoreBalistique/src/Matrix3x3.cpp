#include "Matrix3x3.h"

float Matrix3x3::determinantSousMatrice(const float& a_, const float& b_, const float& c_, const float& d_)
{
	return (a_ * d_) -( b_ * c_);
}

const float& Matrix3x3::get(char indice) const
{
	int indice_ = indice - 97;
	if (indice_ >= 0 && indice_ < 9)
		return get(indice_);
}

Matrix3x3::Matrix3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
	matrix.reserve(9);
	matrix.push_back(a);
	matrix.push_back(b);
	matrix.push_back(c);
	matrix.push_back(d);
	matrix.push_back(e);
	matrix.push_back(f);
	matrix.push_back(g);
	matrix.push_back(h);
	matrix.push_back(i);
}

Matrix3x3::Matrix3x3(std::vector<float> vecInit)
{
	matrix.reserve(9);
	for (int i = 0; i < vecInit.size() && i<9; i++)
	{
		matrix.push_back(vecInit.at(i));
	}
	if (vecInit.size() < 9)
	{
		for (int i = vecInit.size(); i < 9;i++)
			matrix.push_back(0.0f);
	}
}

Matrix3x3::Matrix3x3(Quaternion quaternion_)
{
	matrix.reserve(9);
	float w = quaternion_.getR();
	float x = quaternion_.getI();
	float y = quaternion_.getJ();
	float z = quaternion_.getK();
	matrix.push_back(1 - 2 * (y * y + z * z));
	matrix.push_back(2 * (x * y + z * w));
	matrix.push_back(2 * (x * z + y * w));
	matrix.push_back(2 * (x * y + z * w));
	matrix.push_back(1 - 2 * (x * x + z * z));
	matrix.push_back(2 * (y * z + x * w));
	matrix.push_back(2 * (x * z + y * w));
	matrix.push_back(2 * (y * z + x * w));
	matrix.push_back(1 - 2*(x * x + y * y));
}

void Matrix3x3::transpose()
{
	// r�utilisation du code pour �viter la redondance malgr� une l�g�re perte de performance
	*this = transposed();
}

Matrix3x3 Matrix3x3::transposed() const
{
	return Matrix3x3(get(0), get(3), get(6), get(1), get(4), get (7), get(2), get(5), get(8));
}

Vector3 Matrix3x3::operator*(const Vector3& vector) const
{
	return Vector3(
		vector.x * matrix[ 0 ] + vector.y * matrix[ 1 ] + vector.z * matrix[ 2 ],
		vector.x * matrix[ 3 ] + vector.y * matrix[ 4 ] + vector.z * matrix[ 5 ],
		vector.x * matrix[ 6 ] + vector.y * matrix[ 7 ] + vector.z * matrix[ 8 ]
	);
}

Vector3 Matrix3x3::transform( const Vector3& vector ) const
{
	return ( *this ) * vector;
}

float Matrix3x3::determinant() const
{
	return (get(0) * get(4) * get(8) + get(1) * get(5) * get(6) + get(2) * get(3) * get(7) - get(0) * get(5) * get(7) - get(1) * get(3) * get(8) - get(2) * get(4) * get(6));
}

bool Matrix3x3::isOrthogonal() const
{
	return (*this * this->transposed()).isIdentity();
}

Matrix3x3 Matrix3x3::inverted() const
{
	if (isInvertible()) {
		Matrix3x3 result(0,0,0,0,0,0,0,0,0);
		float a_ = result.determinantSousMatrice(get(4), get(5), get(7), get(8));
		float b_ = -result.determinantSousMatrice(get(3), get(5), get(6), get(8));
		float c_ = result.determinantSousMatrice(get(3), get(4), get(6), get(7));
		float d_ = -result.determinantSousMatrice(get(1), get(2), get(7), get(8));
		float e_ = result.determinantSousMatrice(get(0), get(2), get(6), get(8));
		float f_ = -result.determinantSousMatrice(get(0), get(1), get(6), get(7));
		float g_ = result.determinantSousMatrice(get(1), get(2), get(4), get(5));
		float h_ = -result.determinantSousMatrice(get(0), get(2), get(3), get(5));
		float i_ = result.determinantSousMatrice(get(0), get(1), get(3), get(4));
		Matrix3x3 resultat(a_, b_, c_, d_, e_, f_, g_, h_, i_);
		resultat.transpose();
		return resultat /= determinant();
	}
	else
		return Matrix3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

Matrix3x3 Matrix3x3::operator*(const float& value) const
{
	return Matrix3x3(get(0)*value, get(1)*value, get(2)*value, get(3)*value, get(4)*value, get(5)*value, get(6)*value, get(7)*value, get(8)*value);
}

Matrix3x3& Matrix3x3::operator*=(const float& value)
{
	return *this = *this * value;
}

Matrix3x3 Matrix3x3::operator/(const float& value) const
{
	return Matrix3x3(get(0) /value, get(1) / value, get(2) / value, get(3) / value, get(4) / value, get(5) / value, get(6) /value, get(7) / value, get(8) / value);
}

Matrix3x3& Matrix3x3::operator/=(const float& value)
{
	return *this = *this / value;
}


Matrix3x3 Matrix3x3::operator*(const Matrix3x3& value) const
{
	return Matrix3x3(get(0) * value.get(0) + get(1) * value.get(3) + get(2) * value.get(6), //a 
		get(0) * value.get(1) + get(1) * value.get(4) + get(2) * value.get(7), //b
		get(0) * value.get(2) + get(1) * value.get(5) + get(2) * value.get(8),//c
		get(3)* value.get(0) + get(4)* value.get(3) + get(5) * value.get(6),//d
		get(3) * value.get(1) +get(4) * value.get(4) +get(5)* value.get(7), //e
		get(3) * value.get(2) +get(4) * value.get(5) +get(5)* value.get(8), //f
		get(6) * value.get(0) +get(7) * value.get(3) +get(8)* value.get(6), //g
		get(6) * value.get(1) +get(7) * value.get(4) +get(8)* value.get(7), //h
		get(6) * value.get(2) +get(7) * value.get(5) +get(8)* value.get(8) //i
		);
	
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& value)
{
	return *this = *this * (value);
}
