#ifndef _MATH3D_LIBRARY__
#define _MATH3D_LIBRARY__
#include<memory.h>
#include<math.h>

typedef float M3DVector3f[3];	//vector of three floats (x, y, z)
typedef float M3DVector4f[4];	//vector of four floats 

//4x4 matrix - column major i.e. X vector is 0, 1, 2, etc.
//	0	4	8	12
//	1	5	9	13
//	2	6	10	14
//	3	7	11	15
typedef float M3DMatrix44f[16];

//constants
#define M3D_PI (3.1415926535897932384626433832795)
#define M3D_PI_DIV_180 (0.017453292519943296)

//degrees to radians conversion
#define m3dDegToRad(x) ((x)*M3D_PI_DIV_180)

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif // !M_PI
#define PI_DIV_180 (M_PI / 180.0)
#define SQR(X) (X * X)
#define NULL_VECTOR M3DVector(0.0, 0.0, 0.0)

//copy vector src into vector dst
inline void m3dCopyVector3(M3DVector3f dst, const M3DVector3f src)
{
	memcpy(dst, src, sizeof(M3DVector3f));
}

//cross product. u x v = result
inline void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
{
	result[0] = u[1] * v[2] - v[1] * u[2];
	result[1] = -u[0] * v[2] + v[0] * u[2];
	result[2] = u[0] * v[1] - v[0] * u[1];
}

//Matrix functions

//load Identity matrix
//implemented in MyMath3D.cpp
void m3dLoadIdentity44(M3DMatrix44f m);

//get/set column
inline void m3dSetMatrxiColumn44(M3DMatrix44f dst, const M3DVector4f src, int column)
{
	memcpy(dst + (4 * column), src, sizeof(float)* 4);
}

//MultMatrix
//Implemented in MyMath3D.cpp
void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b);

//Transform - Does rotation and translation via 4x4 matrix. Transforms a point vector
inline void m3dTransformVector3(M3DVector3f vec, const M3DVector3f v, const M3DMatrix44f m)
{
	vec[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12];
	vec[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13];
	vec[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];
}

//Create a rotation matrix
//Implemented in MyMath3D.cpp
void m3dRotationMatrix44(M3DMatrix44f mat, float angle, float x, float y, float z);

//create a translation matrix.
inline void m3dTranslationMatrix44(M3DMatrix44f m, float x, float y, float z)
{
	m3dLoadIdentity44(m);
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

//scale Vectors in place
inline void m3dScaleVector3(M3DVector3f v, float scale)
{
	v[0] *= scale;
	v[1] *= scale;
	v[2] *= scale;
}

//get square of a vector length, only for three component vectors
inline float m3dGetVectorLengthSquared(const M3DVector3f u)
{
	return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}

//get length of vector
//only for three component vector
inline float m3dGetvectorLength(const M3DVector3f u)
{
	return float(sqrt(double(m3dGetVectorLengthSquared(u))));
}

//Normalize a vector
//Scale a vector to unit length. Easy, just scale the vector by it's length
inline void m3dNormalizeVector(M3DVector3f u)
{
	m3dScaleVector3(u, 1.0f / m3dGetvectorLength(u));
}

// Get plane equation from three points and a normal
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3);

// Planar shadow Matrix
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos);

//misc. functions
//find a normal from three points
//implemented in MyMath3D.cpp
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2, const M3DVector3f point3);

#endif
