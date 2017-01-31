#pragma once

#include <math.h>

//=====================================================================
// ???????????????? D3D ??????
//=====================================================================

extern int CMID(int x, int min, int max);

// ?????t ? [0, 1] ?????
extern float _interp(float x1, float x2, float t);

class vector_t
{ 
public :

	float x, y, z, w; 

	vector_t() :x(0), y(0), z(0), w(1.0f) {}

	vector_t(float _x, float _y, float _z) :x(_x), y(_y), z(_z),w(1.0f) {}

	vector_t(float _x, float _y, float _z , float _w) :x(_x), y(_y), z(_z), w(_w) {}

	// | v |
	float length() {
		float sq =x * x + y * y + z * z;
		return (float)sqrt(sq);
	}

	bool operator==(const vector_t & _v)
	{
		return x == _v.x && y == _v.y && z == _v.z && w == _v.w;
	}

	vector_t operator+(const vector_t & _v)
	{
		return vector_t(x + _v.x, y + _v.y, z + _v.z);
	}

	vector_t operator-(const vector_t & _v)
	{
		return vector_t(x - _v.x, y - _v.y, z - _v.z);
	}

	float dot(const vector_t & _v)
	{
		return x * _v.x + y * _v.y + z * _v.z;
	}

	vector_t crossproduct(const vector_t &_v) {
		float m1, m2, m3;
		m1 = y * _v.z - z * _v.y;
		m2 = z * _v.x - x * _v.z;
		m3 = x * _v.y - y * _v.x;
		return vector_t(m1, m2, m3);
	}

	// ?????t?? [0, 1]
	vector_t interp(const vector_t &_v, float t) {
		return vector_t(_interp(x, _v.x, t), _interp(y, _v.y, t), _interp(z, _v.z, t));
	}

	// ?????
	void normalize() {
		float l = length();
		if (l != 0.0f) {
			float inv = 1.0f / l;
			x *= inv;
			y *= inv;
			z *= inv;
		}
	}

} ;

typedef vector_t point_t;

class matrix_t{ 
public :
	float m[4][4]; 

	matrix_t operator+(const matrix_t& Other) const
	{
		matrix_t ResultMat;

		for (int X = 0; X < 4; X++)
		{
			for (int Y = 0; Y < 4; Y++)
			{
				ResultMat.m[X][Y] = m[X][Y] + Other.m[X][Y];
			}
		}

		return ResultMat;
	}

	matrix_t operator-(const matrix_t& Other) const
	{
		matrix_t ResultMat;

		for (int X = 0; X < 4; X++)
		{
			for (int Y = 0; Y < 4; Y++)
			{
				ResultMat.m[X][Y] = m[X][Y] - Other.m[X][Y];
			}
		}

		return ResultMat;
	}

	// c = a * b
	matrix_t operator*(const matrix_t &b) {
		matrix_t z;
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				z.m[j][i] = (m[j][0] * b.m[0][i]) +
					(m[j][1] * b.m[1][i]) +
					(m[j][2] * b.m[2][i]) +
					(m[j][3] * b.m[3][i]);
			}
		}
		return z;
	}

	// c = a * f
	matrix_t operator*(float f) {
		matrix_t z;
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++)
				z.m[i][j] = m[i][j] * f;
		}
		return z;
	}

	// y = x * m
	vector_t apply(const vector_t &x) {
		vector_t y;
		float X = x.x, Y = x.y, Z = x.z, W = x.w;
		y.x = X * m[0][0] + Y * m[1][0] + Z * m[2][0] + W * m[3][0];
		y.y = X * m[0][1] + Y * m[1][1] + Z * m[2][1] + W * m[3][1];
		y.z = X * m[0][2] + Y * m[1][2] + Z * m[2][2] + W * m[3][2];
		y.w = X * m[0][3] + Y * m[1][3] + Z * m[2][3] + W * m[3][3];
		return y;
	}

	void set_identity() {
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
		m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
	}

	void set_zero() {
		m[0][0] = m[0][1] = m[0][2] = m[0][3] = 0.0f;
		m[1][0] = m[1][1] = m[1][2] = m[1][3] = 0.0f;
		m[2][0] = m[2][1] = m[2][2] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0.0f;
	}

	// ????
	void set_translate(float x, float y, float z) {
		set_identity();
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	// ????
	void set_scale( float x, float y, float z) {
		set_identity();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
	}

	// ????
	void set_rotate(float x, float y, float z, float theta) {
		float qsin = (float)sin(theta * 0.5f);
		float qcos = (float)cos(theta * 0.5f);
		vector_t vec ( x, y, z, 1.0f );
		float w = qcos;
		vec.normalize();
		x = vec.x * qsin;
		y = vec.y * qsin;
		z = vec.z * qsin;
		m[0][0] = 1 - 2 * y * y - 2 * z * z;
		m[1][0] = 2 * x * y - 2 * w * z;
		m[2][0] = 2 * x * z + 2 * w * y;
		m[0][1] = 2 * x * y + 2 * w * z;
		m[1][1] = 1 - 2 * x * x - 2 * z * z;
		m[2][1] = 2 * y * z - 2 * w * x;
		m[0][2] = 2 * x * z - 2 * w * y;
		m[1][2] = 2 * y * z + 2 * w * x;
		m[2][2] = 1 - 2 * x * x - 2 * y * y;
		m[0][3] = m[1][3] = m[2][3] = 0.0f;
		m[3][0] = m[3][1] = m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	// ?????
	void matrix_set_lookat(vector_t &eye, vector_t &at, vector_t &up) {
		vector_t xaxis, yaxis, zaxis;

		zaxis = at - eye;
		zaxis.normalize();
		xaxis = up.crossproduct(zaxis);
		xaxis.normalize();
		yaxis = zaxis.crossproduct(xaxis);

		m[0][0] = xaxis.x;
		m[1][0] = xaxis.y;
		m[2][0] = xaxis.z;
		m[3][0] = -xaxis.dot(eye);

		m[0][1] = yaxis.x;
		m[1][1] = yaxis.y;
		m[2][1] = yaxis.z;
		m[3][1] = -yaxis.dot(eye);

		m[0][2] = zaxis.x;
		m[1][2] = zaxis.y;
		m[2][2] = zaxis.z;
		m[3][2] = -zaxis.dot(eye);

		m[0][3] = m[1][3] = m[2][3] = 0.0f;
		m[3][3] = 1.0f;
	}

	// D3DXMatrixPerspectiveFovLH
	void matrix_set_perspective(float fovy, float aspect, float zn, float zf) {
		float fax = 1.0f / (float)tan(fovy * 0.5f);
		set_zero();
		m[0][0] = (float)(fax / aspect);
		m[1][1] = (float)(fax);
		m[2][2] = zf / (zf - zn);
		m[3][2] = -zn * zf / (zf - zn);
		m[2][3] = 1;
	}
};


