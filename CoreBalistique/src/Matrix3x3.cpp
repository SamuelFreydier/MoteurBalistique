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
	// réutilisation du code pour éviter la redondance malgré une légère perte de performance
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

Vector3 Matrix3x3::transformTranspose( const Vector3& vector ) const
{
	return Vector3(
		vector.x * matrix[ 0 ] + vector.y * matrix[ 3 ] + vector.z * matrix[ 6 ],
		vector.x * matrix[ 1 ] + vector.y * matrix[ 4 ] + vector.z * matrix[ 7 ],
		vector.x * matrix[ 2 ] + vector.y * matrix[ 5 ] + vector.z * matrix[ 8 ]
	);
}

Vector3 Matrix3x3::leftTransform(const Vector3& vector) const
{
	return Vector3(
		vector.x * matrix[0] + vector.y * matrix[3] + vector.z * matrix[6],
		vector.x * matrix[1] + vector.y * matrix[4] + vector.z * matrix[7],
		vector.x * matrix[2] + vector.y * matrix[5] + vector.z * matrix[8]
	);
}

void Matrix3x3::setInverse( const Matrix3x3& m )
{
	float t4 = m.matrix[ 0 ] * m.matrix[ 4 ];
	float t6 = m.matrix[ 0 ] * m.matrix[ 5 ];
	float t8 = m.matrix[ 1 ] * m.matrix[ 3 ];
	float t10 = m.matrix[ 2 ] * m.matrix[ 3 ];
	float t12 = m.matrix[ 1 ] * m.matrix[ 6 ];
	float t14 = m.matrix[ 2 ] * m.matrix[ 6 ];

	// Déterminant
	float det = ( t4 * m.matrix[ 8 ] - t6 * m.matrix[ 7 ] - t8 * m.matrix[ 8 ] +
				 t10 * m.matrix[ 7 ] + t12 * m.matrix[ 5 ] - t14 * m.matrix[ 4 ] );

	// Déterminant != 0
	if( det == ( float ) 0.0f ) return;
	float t17 = 1 / det;

	matrix[ 0 ] = ( m.matrix[ 4 ] * m.matrix[ 8 ] - m.matrix[ 5 ] * m.matrix[ 7 ] ) * t17;
	matrix[ 1 ] = -( m.matrix[ 1 ] * m.matrix[ 8 ] - m.matrix[ 2 ] * m.matrix[ 7 ] ) * t17;
	matrix[ 2 ] = ( m.matrix[ 1 ] * m.matrix[ 5 ] - m.matrix[ 2 ] * m.matrix[ 4 ] ) * t17;
	matrix[ 3 ] = -( m.matrix[ 3 ] * m.matrix[ 8 ] - m.matrix[ 5 ] * m.matrix[ 6 ] ) * t17;
	matrix[ 4 ] = ( m.matrix[ 0 ] * m.matrix[ 8 ] - t14 ) * t17;
	matrix[ 5 ] = -( t6 - t10 ) * t17;
	matrix[ 6 ] = ( m.matrix[ 3 ] * m.matrix[ 7 ] - m.matrix[ 4 ] * m.matrix[ 6 ] ) * t17;
	matrix[ 7 ] = -( m.matrix[ 0 ] * m.matrix[ 7 ] - t12 ) * t17;
	matrix[ 8 ] = ( t4 - t8 ) * t17;
}

void Matrix3x3::setSkewSymmetric( const Vector3& vector )
{
	matrix[ 0 ] = matrix[ 4 ] = matrix[ 8 ] = 0;
	matrix[ 1 ] = -vector.z;
	matrix[ 2 ] = vector.y;
	matrix[ 3 ] = vector.z;
	matrix[ 5 ] = -vector.x;
	matrix[ 6 ] = -vector.y;
	matrix[ 7 ] = vector.x;
}

void Matrix3x3::setComponents( const Vector3& compOne, const Vector3& compTwo, const Vector3& compThree )
{
	matrix[ 0 ] = compOne.x;
	matrix[ 1 ] = compTwo.x;
	matrix[ 2 ] = compThree.x;
	matrix[ 3 ] = compOne.y;
	matrix[ 4 ] = compTwo.y;
	matrix[ 5 ] = compThree.y;
	matrix[ 6 ] = compOne.z;
	matrix[ 7 ] = compTwo.z;
	matrix[ 8 ] = compThree.z;
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

void Matrix3x3::operator+=( const Matrix3x3& o )
{
	matrix[ 0 ] += o.matrix[ 0 ]; matrix[ 1 ] += o.matrix[ 1 ]; matrix[ 2 ] += o.matrix[ 2 ];
	matrix[ 3 ] += o.matrix[ 3 ]; matrix[ 4 ] += o.matrix[ 4 ]; matrix[ 5 ] += o.matrix[ 5 ];
	matrix[ 6 ] += o.matrix[ 6 ]; matrix[ 7 ] += o.matrix[ 7 ]; matrix[ 8 ] += o.matrix[ 8 ];
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
