// CMakeDirectX12.cpp : Defines the entry point for the application.
//

#include "CMakeDirectX12.h"
#include <DirectXMath.h> // To use DirectX Math library, namespace DirectX
using namespace DirectX; 
#include <DirectXPackedVector.h> // Additional data files, namespace DirectX::PackedVector
using namespace DirectX::PackedVector; 

// also we should enable enchanced instruction set and fast floating point model

// XMVector is the vector type that maps to SIMD hardware that can process four 32 bit flaots with a single SIMD instruction
//typedef _m128 XMVECTOR; // I forgot how typedef works
// XMVECTOR needs to be 16-byte aligned, done automaitcally for local/global variables
// But for class data members, need to use XMFLOAT2 for 2d, XMFLOAT3 for 3D, and XMFLOAT4 for 4D, 
using namespace std;

// struct XMFLOAT2 {
//	float x; 
//	float y; 
//
//	XMFLOAT2() {}
//	XMFLOAT2(float _x, float _y) : x(_x), y(_y) {}
//
//	explicit XMFLOAT2 (_In_reads_(2) const float *pArray) : 
//		x(pArray[0]), y(pArray[1]) {}
//
//	XMFLOAT2& operator= (const XMFLOAT2& Float2) {
//		x = Float2.x; 
//		y = Float2.y; 
//		
//		return *this;
//	}
//};

//struct XMFLOAT3 {
//	float x;
//	float y;
//	float z;
//
//	XMFLOAT3() {}
//
//	XMFLOAT3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//	explicit XMFLOAT3(_In_reads_(3) const float* pArray) :
//		x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
//
//	XMFLOAT3& operator= (const XMFLOAT3& Float3) {
//		x = Float3.x;
//		y = Float3.y;
//		z = Float3.z;
//
//		return *this;
//	}
//
//};
//
//struct XMFLOAT4 {
//	float x; 
//	float y;
//	float z;
//	float w;
//
//	XMFLOAT4() {}
//
//	XMFLOAT4(float _x, float _y, float _z, float _w) {}
//
//	explicit XMFLOAT4(_In_reads_(4) const float * pArray) :
//		x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
//
//	XMFLOAT4& operator= (const XMFLOAT4& Float4) {
//		x = Float4.x; 
//		y = Float4.y; 
//		z = Float4.z;
//		w = Float4.w;
//
//		return *this;
//	}
//};

// Load float2 into MVECTOR
XMVECTOR XM_CALLCONV XMLoadFloat2(const XMFLOAT2* pSource);

XMVECTOR XM_CALLCONV XMLoadFloat3(const XMFLOAT3* pSource);

XMVECTOR XM_CALLCONV XMLoadFloat4(const XMFLOAT4* pSource);

// store data from XMVector into XMFLoat n

void XM_CALLCONV XMStoreFloat2(XMFLOAT2* pDestination, FXMVECTOR v);
void XM_CALLCONV XMStoreFloat3(XMFLOAT3* pDestination, FXMVECTOR v);
void XM_CALLCONV XMStoreFloat4(XMFLOAT4* pDestination, FXMVECTOR v);


// getter/setter functions for one component

// XM_CALLCONV makes sure we're using the right calling convention
float XM_CALLCONV XMVectorGetX(FXMVECTOR v); // uses FXMVECTOR
float XM_CALLCONV XMVectorGetY(FXMVECTOR v); // uses FXMVECTOR
float XM_CALLCONV XMVectorGetZ(FXMVECTOR v); // uses FXMVECTOR
float XM_CALLCONV XMVectorGetW(FXMVECTOR v); // Should be GXMVECTOR? not sure why still FXMVECTOR
// then HXMVECTOR
// HXMVECTOR
// CXMVECTOR

XMVECTOR XM_CALLCONV XMVectorSetX(FXMVECTOR V, float x); 
XMVECTOR XM_CALLCONV XMVectorSetY(FXMVECTOR v, float y);
XMVECTOR XM_CALLCONV XMVectorSetZ(FXMVECTOR v, float z);
XMVECTOR XM_CALLCONV XMVectorSetW(FXMVECTOR v, float w);

// XMVECtor values can be passed as aruments fo functions in the SSE registers instead of on the stack
// Number of arguments depends on the plataform and complier — so to make sure we get the number of arguments right, 
// use FXMVECTOR, GXMVECTOR, HXMVECTOR, and CXMVECTOR to pass XMVECTOR parameters
// // defined to the right type based on platform and complier
// // calling convention annotation XM_CALLCONV (call convention?) must be specified before the function name so the proper calling convention is used
// 
// using above types directly does not take advantage of SIMED, need to convert to XMVECTOR type to do so
// Done with DirectX Math loading functions — can also convert the other way from XMVECTOR to XMFLOATn types above


// on 32 bit windows with __fastcall compiler, the first three are XMVVECTOR via registers, rest on stack
typedef const XMVECTOR FXMVECTOR; // via registers
typedef const XMVECTOR& GXMVECTOR; // on stack
typedef const XMVECTOR& HXMVECTOR; // on stack
typedef const XMVECTOR& CXMVECTOR; // on stack

// but for 32-bit windows __vectorcall compilers, first 7 via registers
typedef const XMVECTOR FXMVECTOR; // via regist
typedef const XMVECTOR GXMVECTOR; // via register
typedef const XMVECTOR HXMVECTOR; // via register
typedef const XMVECTOR& CXMMVECTOR; // on stack

// details on definition under Library Internals in DirectX Math documentation[DirectXMath]. 
// But constructor that takes XMVECTOR parameters recomments using FXVECTOR for first three and CXMVECTOR for rest 
// also don't use XM_CALLCONV for constructors (presumably for above reasons? to not use call conv based on platform/compiler)

// If theres' non XMVECTOR parameters between XMVECTOR parameters, use the rules as if they're not there
// also only applies to input parameters. Outputs of XMVECTOR& or XMVECTOR* wiwll not use registers, so treat like non XMVECTOORparameters

// All that was for variables. Constant vectors should use XMVECTORF32 type eg. in cascadedd shaddow maps
// defined

_declspec(align(16)) struct XMVECTORF32 { // declspec (declare spec?) allows attributing type/function with storage class information
	union {
		float f[4];
		XMVECTOR v;
	};

	inline operator XMVECTOR() const { return v; }
	inline operator const float* () { return f; }
#if !defined(_XM_NO_INTRENSICS_) && defined(_XM_SSE_INTRINSICS)
	inline opreator __m128i() const { return _mm_castps_sil28(v); }
	inline operator __m128dd() const { return _mm_castps_pd(v); }
#endif
};

// create const XMVECTOR of integer data using XMVECTORU32??
static const XMVECTORU32 vGrabY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x0000000 };

// XMVECTOR also has several overloaded operators for vector operations 
XMVECTOR XM_CALLCONV operator+(FXMVECTOR V);
XMVECTOR XM_CALLCONV operator-(FXMVECTOR V);

XMVECTOR& XM_CALLCONV operator+= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR& XM_CALLCONV operator-= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR& XM_CALLCONV operator*= (XMVECTOR& V1, FXMVECTOR V2);
XMVECTOR& XM_CALLCONV operator/= (XMVECTOR& V1, FXMVECTOR V2);

XMVECTOR& operator*= (XMVECTOR& V, float S);
XMVECTOR& operator/= (XMVECTOR& V, float S);

XMVECTOR XM_CALLCONV operator+ (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR XM_CALLCONV operator- (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR XM_CALLCONV operator* (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR XM_CALLCONV operator/ (FXMVECTOR V1, FXMVECTOR V2);
XMVECTOR XM_CALLCONV operator* (FXMVECTOR V1, float S);
XMVECTOR XM_CALLCONV operator* (float S, FXMVECTOR V2);
XMVECTOR XM_CALLCONV operator/ (FXMVECTOR V1, float S);

// lol directX math library also has variour PI apprximations including pi, 2pi, 1/pi, 1/2pi, pi/2, pi/4
// also converting between radians and degrees fucntionx, float XMConvertToRadiuans(float fDegrees), and dmin/max XMMin


// Setter fnuctions, that returns 0 vector, (1, 1, 1, 1) vector, set the vector, set all vector values same, set all to x/y/z
// also magnitude, dot product, cross, normalized, orthogonal, angle between, perp/prorjection, whether emqual or not — 
// any that returns a scalar still returns a vector with all components the same scalar value as mixing scalar/vector operations is less efficient


// Floating point errors....... even a normalized vector will have a length of Approximately 1
// and error accumulates. So directX Math provides XMVector3NearEqual that has an Epsilon parameter


// MATRICES 
// determinant provides information on how volumes change under linear transformations + solve systems of linear equations
// but also allows finding inverse of a matrix, and invertible iff det A != 0

// recall matrix minor A_ij (with a line on) ie. delete ith row and jth column

// recacall cofactor of a matrix minor, defined C_ij = (-1)^(i+j) matrix minor A_ij


// directX math uses the XMMatrix class

//struct XMMATRIX{ // or delcspect(align(16)) struct XMMatrix 
//__declspec(align(16)) struct XMMATRIX {
//	// use 4 XMVectors to represent the matrix for XIMD 
//	XMVECTOR r[4]; 
//
//	XMMATRIX() {}
//
//	// Initialize the matrix by specifiying 4 row vectors 
//	XMMATRIX(FXMVECTOR R0, FXMVECTOR R1, FXMVECTOR R2, CXMVECTOR R3) { // uses four XMMVECTOR instances to use SIMD 
//		r[0] = R0; r[1] = R1; r[2] = R2; r[3] = R3;
//	}
//
//	// initialize the matrix by specifying four row vector in floats
//	XMMATRIX(float m00, float m01, float m02, float m03,
//		float m10, float m11, float m12, float m13,
//		float m20, float m21, float m22, float m23,
//		float m30, float m31, float m32, float m33);
//
//	explicit XMMATRIX(_In_reads_(16) const float* pArray);
//
//	XMMATRIX XM_CALLCONV XMMatrixSet(float m00, float m01, float mm02, float m03,
//		float m10, float m11, float m12, float m13,
//		float m20, float m21, float m22, float m23,
//		float m30, float m31, float m32, float m33); // constructor using the XMMatrixSet
//
//	XMMATRIX& operator= (const XMMATRIX& M) {
//		r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2], r[3] = M.r[3];  // overload operators for the matrix arithmetic
//
//		return *this;
//	}
//
//	XMMATRIX operator+ () const { return *this; }
//	XMMATRIX operator-() const;
//
//	XMMATRIX& XM_CALLCONV operator+= (FXMMATRIX M);
//	XMMATRIX& XM_CALLCONV operator-= (FXMMATRIX M);
//	XMMATRIX& XM_CALLCONV operator*= (FXMMATRIX M);
//
//	XMMATRIX operator* (float S) const;
//	XMMATRIX operator/ (float S) const;
//
//	friend XMMATRIX XM_CALLCONV operator* (float S, FXMMATRIX M);
//};
//
///// also just as we had XMFLOAT2, XMFLOAT3, and XMFLOAT4, recommended to use XMFLOAT4X4 type to store matrices as class data members
//struct XMFLOAT4X4 {
//	union {
//		struct {
//			float _11, _12, _13, _14;
//			float _21, _22, _23, _24;
//			float _31, _32, _33, _34;
//			float _41, _42, _43, _44;
//		};
//		float m[4][4];
//	};
//
//	XMFLOAT4X4() {}
//	XMFLOAT4X4(float m00, float m01, float m02, float m03,
//		float m10, float m11, float m12, float m13,
//		float m20, float m21, float m22, float m23,
//		float m30, float m31, float m32, float m33);
//
//	explicit XMFLOAT4X4(_In_reads_(16) const float* pArray);
//
//	float operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
//
//	float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
//
//	XMFLOAT4X4& operator= (const XMFLOAT4X4& Float4x4);
//};

// to load data from XMFLOAT4X4 to XMMATRIX
inline XMMATRIX XM_CALLCONV XMLoadFloat4x4(const XMFLOAT4X4* pSource);

// Store data from XMMATRIX into XMFLOAT4X4 
inline void XM_CALLCONV XMStoreFloat4x4(XMFLOAT4X4 * pDestination, FXMMATRIX M);

// DirectX math library includes 
// XMMATRIX XM_CALLCONV XMMatrixIdentity(); returns identity matrix; 

// boolean XM_CALLCONV XMMatrixIsIdentity(FXMMatrix M); returns if M is identity
// XMMATRIX XM_CALLCONV AMMatrixMultiply(FXMMATRIX A, CXMMATRIX B)
// XMMATRIX XM_CALLCONV XMMatrixTranspose(FXMMATRIX M);
// XMVECTOR XM_CALLCONV XMMatrixDeterminant(FXMMATRIX M); 
// XMMATRIX XM_CALLCONV XMMatrixInverse(XMVECTOR( pDeterminant, FXMMATRIX M);


int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
