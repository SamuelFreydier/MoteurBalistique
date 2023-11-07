#include "Matrix4x4.h"

const float& Matrix4x4::Matrix4x4::get(char indice) const
{
	int indice_ = indice - 97;
	if (indice_ >= 0 && indice_ < 16)
		return get(indice_);
}

Matrix4x4::Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
{
	matrix.reserve(16);
	matrix.push_back(a);
	matrix.push_back(b);
	matrix.push_back(c);
	matrix.push_back(d);
	matrix.push_back(e);
	matrix.push_back(f);
	matrix.push_back(g);
	matrix.push_back(h);
	matrix.push_back(i);
	matrix.push_back(j);
	matrix.push_back(k);
	matrix.push_back(l);
	matrix.push_back(m);
	matrix.push_back(n);
	matrix.push_back(o);
	matrix.push_back(p);
}

Matrix4x4::Matrix4x4(std::vector<float> vecInit)
{
	{
		matrix.reserve(16);
		for (int i = 0; i < vecInit.size() && i < 16; i++)
		{
			matrix.push_back(vecInit.at(i));
		}
		if (vecInit.size() < 16)
		{
			for (int i = vecInit.size(); i < 16;i++)
				matrix.push_back(0.0f);
		}
	}
}

void Matrix4x4::transpose()
{
	*this = transposed();
}

Matrix4x4 Matrix4x4::transposed() const
{
	return Matrix4x4(get(0), get(4), get(8), get(12), get(1), get(5), get(9), get(13), get(2), get(6), get(10), get(14), get(3), get(7), get(11), get(15));
}

Matrix4x4 Matrix4x4::operator*(const vector<float>& value) const
{
	return Matrix4x4(get(0)*value.at(0), get(1)*value.at(1), get(2)*value.at(2), get(3)*value.at(3), get(4)*value.at(0), get(5)*value.at(1), get(6)*value.at(2), get(7)*value.at(3), get(8)*value.at(0), get(9)*value.at(1), get(10)*value.at(2), get(11)*value.at(3), get(12)*value.at(0), get(13)*value.at(1), get(14)*value.at(2), get(15)*value.at(3));
}

float Matrix4x4::determinant() const
{
	float deter = get(0) * Matrix3x3(get(5), get(6), get(7), get(9), get(10), get(11), get(13), get(14), get(15)).determinant();
	deter -= get(1) * Matrix3x3(get(4), get(6), get(7), get(8), get(10), get(11), get(12), get(14), get(15)).determinant();
	deter += get(2) * Matrix3x3(get(4), get(5), get(7), get(8), get(9), get(11), get(12), get(13), get(15)).determinant();
	deter -= get(3) * Matrix3x3(get(4), get(5), get(6), get(8), get(9), get(10), get(12), get(13), get(14)).determinant();
	return deter;
}

bool Matrix4x4::isOrthogonal() const
{
	return (*this * this->transposed()).isIdentity();
}

Matrix4x4 Matrix4x4::inverted() const
{
	if (isInvertible()) {
		float a_ = Matrix3x3(get(5), get(6), get(7), get(9), get(10), get(11), get(13), get(14), get(15)).determinant();
		float b_ = -Matrix3x3(get(4), get(6), get(7), get(8), get(10), get(11), get(12), get(14), get(15)).determinant();
		float c_ = Matrix3x3(get(4), get(5), get(7), get(8), get(9), get(11), get(12), get(13), get(15)).determinant();
		float d_ = -Matrix3x3(get(4), get(5), get(6), get(8), get(9), get(10), get(12), get(13), get(14)).determinant();

		float e_ = -Matrix3x3(get(1), get(2), get(3), get(9), get(10), get(11), get(13), get(14), get(15)).determinant();
		float f_ = Matrix3x3(get(0), get(2), get(3), get(8), get(10), get(11), get(12), get(14), get(15)).determinant();
		float g_ = -Matrix3x3(get(0), get(1), get(3), get(8), get(9), get(11), get(12), get(13), get(15)).determinant();
		float h_ = Matrix3x3(get(0), get(1), get(2), get(8), get(9), get(10), get(12), get(13), get(14)).determinant();

		float i_ = Matrix3x3(get(1), get(2), get(3), get(5), get(6), get(7), get(13), get(14), get(15)).determinant();
		float j_ =-Matrix3x3(get(0), get(2), get(3), get(4), get(6), get(7), get(12), get(14), get(15)).determinant();
		float k_ = Matrix3x3(get(0), get(1), get(3), get(4), get(5), get(7), get(12), get(13), get(15)).determinant();
		float l_ = -Matrix3x3(get(0), get(1), get(2), get(4), get(5), get(6), get(12), get(13), get(14)).determinant();

		float m_ = -Matrix3x3(get(1), get(2), get(3), get(5), get(6), get(7), get(9), get(10), get(11)).determinant();
		float n_ = Matrix3x3(get(0), get(2), get(3), get(4), get(6), get(7), get(8), get(10), get(11)).determinant();
		float o_ = -Matrix3x3(get(0), get(1), get(3), get(4), get(5), get(7), get(8), get(9), get(11)).determinant();
		float p_ = Matrix3x3(get(0), get(1), get(2), get(4), get(5), get(6), get(8), get(9), get(10)).determinant();
		Matrix4x4 result(a_, b_, c_, d_, e_, f_, g_, h_, i_, j_, k_, l_, m_, n_, o_,p_);
		result.transpose();
		return result /= determinant();
	}
	else
		return Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
}

Matrix4x4 Matrix4x4::operator*(const float& value) const
{
	return Matrix4x4(get(0) * value, get(1) * value, get(2) * value, get(3) * value, get(4) * value, get(5) * value, get(6) * value, get(7) * value, get(8) * value, get(9)*value, get(10)*value, get(11) * value, get(12) * value, get(13) * value, get(14)*value, get(15)*value);
}

Matrix4x4& Matrix4x4::operator*=(const float& value)
{
	return *this = *this * value;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& value) const
{
	std::vector<float> res;
	res.reserve(16);
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float tmp = 0;
			for (int k = 0; k < 4; k++)
			{
				tmp += get(4 * i + k) * value.get(j + 4 * k); //plus facile, rapide et intelligent à taper que d'écrire toutes les combinaisons à la main
			}
			res.push_back(tmp);
		}
	}
	return Matrix4x4(res);
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& value)
{
	return *this = *this * value;
}

Matrix4x4 Matrix4x4::operator/(const float& value) const
{
	return Matrix4x4(get(0) / value, get(1) / value, get(2) / value, get(3) / value, get(4) / value, get(5) / value, get(6) / value, get(7) / value, get(8) / value, get(9) / value, get(10) / value, get(11) / value, get(12) / value, get(13) / value, get(14) / value, get(15) / value);
}

Matrix4x4& Matrix4x4::operator/=(const float& value)
{
	return *this = *this * value;
}
