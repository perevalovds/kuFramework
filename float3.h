#pragma once

#include "float2.h"
//#include "kuVec4f.h"
#include "kuConstants.h"

#include <cmath>
#include <iostream>

class float3 {
public:
	float x,y,z;
    
    static const int DIM = 3;
	
	float3();
	float3( float _x, float _y, float _z=0 );
	/// assigns scalar to x, y, and z
	explicit float3( float _scalar );
	
    float3( const float2& vec );
    //float3( const ofVec4f& vec );
	
	float * getPtr() {
		return (float*)&x;
	}
	const float * getPtr() const {
		return (const float *)&x;
	}
	
	float& operator[]( int n ){
		return getPtr()[n];
	}
	
	float operator[]( int n ) const {
		return getPtr()[n];
	}
	
    
	// Getters and Setters.
    //
    void set( float _x, float _y, float _z = 0 );
    void set( const float3& vec );
	void set( float _scalar );
	
    // Check similarity/equality.
    //
    bool operator==( const float3& vec ) const;
    bool operator!=( const float3& vec ) const;
    bool match( const float3& vec, float tolerance=0.0001 ) const;
    /**
	 * Checks if vectors look in the same direction.
	 */
    bool isAligned( const float3& vec, float tolerance=0.0001 ) const;
    bool align( const float3& vec, float tolerance=0.0001 ) const;
    bool isAlignedRad( const float3& vec, float tolerance=0.0001 ) const;
    bool alignRad( const float3& vec, float tolerance=0.0001 ) const;
	
	
    // Operator overloading for float3
    //
    float3  operator+( const float3& pnt ) const;
    float3& operator+=( const float3& pnt );
    float3  operator-( const float3& vec ) const;
    float3& operator-=( const float3& vec );
    float3  operator*( const float3& vec ) const;
    float3& operator*=( const float3& vec );
    float3  operator/( const float3& vec ) const;
    float3& operator/=( const float3& vec );
    float3  operator-() const;
	
    //operator overloading for float
    //
	//    void 	  operator=( const float f);
    float3  operator+( const float f ) const;
    float3& operator+=( const float f );
 	float3  operator-( const float f ) const;
    float3& operator-=( const float f );
    float3  operator*( const float f ) const;
    float3& operator*=( const float f );
    float3  operator/( const float f ) const;
    float3& operator/=( const float f );
	
	friend ostream& operator<<(ostream& os, const float3& vec);
	friend istream& operator>>(istream& is, const float3& vec);
	
    //Scale
    //
    float3  getScaled( const float length ) const;
    float3& scale( const float length );
    
	
    // Rotation
    //
    float3  getRotated( float angle, const float3& axis ) const;
    float3  getRotatedRad( float angle, const float3& axis ) const;
    float3& rotate( float angle, const float3& axis );
    float3& rotateRad( float angle, const float3& axis );
    float3  getRotated(float ax, float ay, float az) const;
    float3  getRotatedRad(float ax, float ay, float az) const;
    float3& rotate(float ax, float ay, float az);
    float3& rotateRad(float ax, float ay, float az);
    
    
    // Rotation - point around pivot
    //    
    float3   getRotated( float angle, const float3& pivot, const float3& axis ) const;
    float3&  rotate( float angle, const float3& pivot, const float3& axis );
    float3   getRotatedRad( float angle, const float3& pivot, const float3& axis ) const;
    float3&  rotateRad( float angle, const float3& pivot, const float3& axis );    
	
	
    // Map point to coordinate system defined by origin, vx, vy, and vz.
    //
    float3 getMapped( const float3& origin,
					  const float3& vx,
					  const float3& vy,
					  const float3& vz ) const;
    float3& map( const float3& origin,
				 const float3& vx,
				 const float3& vy,
				 const float3& vz );
	
	
    // Distance between two points.
    //
    float distance( const float3& pnt) const;
    float squareDistance( const float3& pnt ) const;
	
	
    // Linear interpolation.
    //
    /**
	 * p==0.0 results in this point, p==0.5 results in the
	 * midpoint, and p==1.0 results in pnt being returned.
	 */
    float3   getInterpolated( const float3& pnt, float p ) const;
    float3&  interpolate( const float3& pnt, float p );
    float3   getMiddle( const float3& pnt ) const;
    float3&  middle( const float3& pnt );
    float3&  average( const float3* points, int num );
    
	
    // Normalization
    //
    float3  getNormalized() const;
    float3& normalize();
	
	
    // Limit length.
    //
    float3  getLimited(float max) const;
    float3& limit(float max);
	
	
    // Perpendicular vector.
    //
    float3  getCrossed( const float3& vec ) const;
    float3& cross( const float3& vec );
    /**
	 * Normalized perpendicular.
	 */
    float3  getPerpendicular( const float3& vec ) const;
    float3& perpendicular( const float3& vec );
	
	
    // Length
    //
    float length() const;
    float lengthSquared() const;
//	OF_DEPRECATED_MSG("Use float3::lengthSquared() instead.", float squareLength() const);
    /**
	 * Angle (deg) between two vectors.
	 * This is an unsigned relative angle from 0 to 180.
	 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	 */
    float angle( const float3& vec ) const;
    float angleRad( const float3& vec ) const;
	
	
    // Dot Product
    //
    float dot( const float3& vec ) const;
	
	
	
    //-----------------------------------------------
    // this methods are deprecated in 006 please use:
	
    // getScaled
    float3 rescaled( const float length ) const;
	
    // scale
    float3& rescale( const float length );
	
    // getRotated
    float3 rotated( float angle, const float3& axis ) const;
	
    // getRotated should this be const???
    float3 rotated(float ax, float ay, float az);
	
    // getNormalized
    float3 normalized() const;
	
    // getLimited
    float3 limited(float max) const;
	
    // getCrossed
    float3 crossed( const float3& vec ) const;
	
    // getPerpendicular
    float3 perpendiculared( const float3& vec ) const;
    
    // use getMapped
    float3  mapped( const float3& origin,
					const float3& vx,
					const float3& vy,
					const float3& vz ) const;
	
    // use squareDistance
    float  distanceSquared( const float3& pnt ) const;
	
    // use getInterpolated
    float3 	interpolated( const float3& pnt, float p ) const;
	
    // use getMiddle
    float3 	middled( const float3& pnt ) const;
    
    // use getRotated
    float3 	rotated( float angle,
						const float3& pivot,
						const float3& axis ) const;    

    // return all zero vector
    static float3 zero() { return float3(0, 0, 0); }
    
    // return all one vector
    static float3 one() { return float3(1, 1, 1); }
};




// Non-Member operators
//
//
float3 operator+( float f, const float3& vec );
float3 operator-( float f, const float3& vec );
float3 operator*( float f, const float3& vec );
float3 operator/( float f, const float3& vec );


/////////////////
// Implementation
/////////////////


inline float3::float3( const float2& vec ):x(vec.x), y(vec.y), z(0) {}
//inline float3::float3( const ofVec4f& vec ):x(vec.x), y(vec.y), z(vec.z) {}
inline float3::float3(): x(0), y(0), z(0) {};
inline float3::float3( float _all ): x(_all), y(_all), z(_all) {};
inline float3::float3( float _x, float _y, float _z ):x(_x), y(_y), z(_z) {}


// Getters and Setters.
//
//
inline void float3::set( float _scalar ) {
	x = _scalar;
	y = _scalar;
	z = _scalar;
}

inline void float3::set( float _x, float _y, float _z ) {
	x = _x;
	y = _y;
	z = _z;
}

inline void float3::set( const float3& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


// Check similarity/equality.
//
//
inline bool float3::operator==( const float3& vec ) const {
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

inline bool float3::operator!=( const float3& vec ) const {
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

inline bool float3::match( const float3& vec, float tolerance ) const{
	return (fabs(x - vec.x) < tolerance)
	&& (fabs(y - vec.y) < tolerance)
	&& (fabs(z - vec.z) < tolerance);
}

/**
 * Checks if vectors look in the same direction.
 */
inline bool float3::isAligned( const float3& vec, float tolerance ) const {
	float angle = this->angle( vec );
	return  angle < tolerance;
}
inline bool float3::align( const float3& vec, float tolerance ) const {
    return isAligned( vec, tolerance );
}

inline bool float3::isAlignedRad( const float3& vec, float tolerance ) const {
	float angle = this->angleRad( vec );
	return  angle < tolerance;
}
inline bool float3::alignRad( const float3& vec, float tolerance ) const {
    return isAlignedRad( vec, tolerance );
}


// Operator overloading for float3
//
//

inline ostream& operator<<(ostream& os, const float3& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

inline istream& operator>>(istream& is, float3& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	return is;
}

inline float3 float3::operator+( const float3& pnt ) const {
	return float3( x+pnt.x, y+pnt.y, z+pnt.z );
}

inline float3& float3::operator+=( const float3& pnt ) {
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

inline float3 float3::operator-( const float3& vec ) const {
	return float3( x-vec.x, y-vec.y, z-vec.z );
}

inline float3& float3::operator-=( const float3& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

inline float3 float3::operator*( const float3& vec ) const {
	return float3( x*vec.x, y*vec.y, z*vec.z );
}

inline float3& float3::operator*=( const float3& vec ) {
	x*=vec.x;
	y*=vec.y;
	z*=vec.z;
	return *this;
}

inline float3 float3::operator/( const float3& vec ) const {
	return float3( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z );
}

inline float3& float3::operator/=( const float3& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	return *this;
}

inline float3 float3::operator-() const {
	return float3( -x, -y, -z );
}


//operator overloading for float
//
//
//inline void float3::operator=( const float f){
//	x = f;
//	y = f;
//	z = f;
//}

inline float3 float3::operator+( const float f ) const {
	return float3( x+f, y+f, z+f);
}

inline float3& float3::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	return *this;
}

inline float3 float3::operator-( const float f ) const {
	return float3( x-f, y-f, z-f);
}

inline float3& float3::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

inline float3 float3::operator*( const float f ) const {
	return float3( x*f, y*f, z*f );
}

inline float3& float3::operator*=( const float f ) {
	x*=f;
	y*=f;
	z*=f;
	return *this;
}

inline float3 float3::operator/( const float f ) const {
	if(f == 0) return float3( x, y, z);
	
	return float3( x/f, y/f, z/f );
}

inline float3& float3::operator/=( const float f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	z/=f;
	return *this;
}


//Scale
//
//
inline float3 float3::rescaled( const float length ) const {
	return getScaled(length);
}
inline float3 float3::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y + z*z);
	if( l > 0 )
		return float3( (x/l)*length, (y/l)*length, (z/l)*length );
	else
		return float3();
}
inline float3& float3::rescale( const float length ) {
	return scale(length);
}
inline float3& float3::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y + z*z);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
	}
	return *this;
}



// Rotation
//
//
inline float3 float3::rotated( float angle, const float3& axis ) const {
	return getRotated(angle, axis);
}
inline float3 float3::getRotated( float angle, const float3& axis ) const {
	float3 ax = axis.normalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	return float3( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

inline float3 float3::getRotatedRad( float angle, const float3& axis ) const {
	float3 ax = axis.normalized();
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	return float3( x*(ax.x*ax.x*cosb + cosa)
				   + y*(ax.x*ax.y*cosb - ax.z*sina)
				   + z*(ax.x*ax.z*cosb + ax.y*sina),
				   x*(ax.y*ax.x*cosb + ax.z*sina)
				   + y*(ax.y*ax.y*cosb + cosa)
				   + z*(ax.y*ax.z*cosb - ax.x*sina),
				   x*(ax.z*ax.x*cosb - ax.y*sina)
				   + y*(ax.z*ax.y*cosb + ax.x*sina)
				   + z*(ax.z*ax.z*cosb + cosa) );
}

inline float3& float3::rotate( float angle, const float3& axis ) {
	float3 ax = axis.normalized();
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}


inline float3& float3::rotateRad(float angle, const float3& axis ) {
	float3 ax = axis.normalized();
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float nx = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float ny = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float nz = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	x = nx; y = ny; z = nz;
	return *this;
}

// const???
inline float3 float3::rotated(float ax, float ay, float az) {
	return getRotated(ax,ay,az);
}

inline float3 float3::getRotated(float ax, float ay, float az) const {
	float a = (float)cos(DEG_TO_RAD*(ax));
	float b = (float)sin(DEG_TO_RAD*(ax));
	float c = (float)cos(DEG_TO_RAD*(ay));
	float d = (float)sin(DEG_TO_RAD*(ay));
	float e = (float)cos(DEG_TO_RAD*(az));
	float f = (float)sin(DEG_TO_RAD*(az));
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return float3( nx, ny, nz );
}

inline float3 float3::getRotatedRad(float ax, float ay, float az) const {
	float a = cos(ax);
	float b = sin(ax);
	float c = cos(ay);
	float d = sin(ay);
	float e = cos(az);
	float f = sin(az);
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	return float3( nx, ny, nz );
}


inline float3& float3::rotate(float ax, float ay, float az) {
	float a = (float)cos(DEG_TO_RAD*(ax));
	float b = (float)sin(DEG_TO_RAD*(ax));
	float c = (float)cos(DEG_TO_RAD*(ay));
	float d = (float)sin(DEG_TO_RAD*(ay));
	float e = (float)cos(DEG_TO_RAD*(az));
	float f = (float)sin(DEG_TO_RAD*(az));
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}


inline float3& float3::rotateRad(float ax, float ay, float az) {
	float a = cos(ax);
	float b = sin(ax);
	float c = cos(ay);
	float d = sin(ay);
	float e = cos(az);
	float f = sin(az);
	
	float nx = c * e * x - c * f * y + d * z;
	float ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	float nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;
	
	x = nx; y = ny; z = nz;
	return *this;
}


// Rotate point by angle (deg) around line defined by pivot and axis.
//
//
inline float3 float3::rotated( float angle,
								const float3& pivot,
								const float3& axis ) const{
	return getRotated(angle, pivot, axis);
}

inline float3 float3::getRotated( float angle,
								   const float3& pivot,
								   const float3& axis ) const
{
	float3 ax = axis.normalized();
	float tx = x - pivot.x;
	float ty = y - pivot.y;
	float tz = z - pivot.z;
	
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return float3( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline float3 float3::getRotatedRad( float angle,
									  const float3& pivot,
									  const float3& axis ) const
{
	float3 ax = axis.normalized();
	float tx = x - pivot.x;
	float ty = y - pivot.y;
	float tz = z - pivot.z;
	
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = tx*(ax.x*ax.x*cosb + cosa)
	+ ty*(ax.x*ax.y*cosb - ax.z*sina)
	+ tz*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = tx*(ax.y*ax.x*cosb + ax.z*sina)
	+ ty*(ax.y*ax.y*cosb + cosa)
	+ tz*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = tx*(ax.z*ax.x*cosb - ax.y*sina)
	+ ty*(ax.z*ax.y*cosb + ax.x*sina)
	+ tz*(ax.z*ax.z*cosb + cosa);
	
	
	return float3( xrot+pivot.x, yrot+pivot.y, zrot+pivot.z );
}


inline float3& float3::rotate( float angle,
								const float3& pivot,
								const float3& axis )
{
	float3 ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	float a = (float)(angle*DEG_TO_RAD);
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}


inline float3& float3::rotateRad( float angle,
								   const float3& pivot,
								   const float3& axis )
{
	float3 ax = axis.normalized();
	x -= pivot.x;
	y -= pivot.y;
	z -= pivot.z;
	
	float a = angle;
	float sina = sin( a );
	float cosa = cos( a );
	float cosb = 1.0f - cosa;
	
	float xrot = x*(ax.x*ax.x*cosb + cosa)
	+ y*(ax.x*ax.y*cosb - ax.z*sina)
	+ z*(ax.x*ax.z*cosb + ax.y*sina);
	float yrot = x*(ax.y*ax.x*cosb + ax.z*sina)
	+ y*(ax.y*ax.y*cosb + cosa)
	+ z*(ax.y*ax.z*cosb - ax.x*sina);
	float zrot = x*(ax.z*ax.x*cosb - ax.y*sina)
	+ y*(ax.z*ax.y*cosb + ax.x*sina)
	+ z*(ax.z*ax.z*cosb + cosa);
	
	x = xrot + pivot.x;
	y = yrot + pivot.y;
	z = zrot + pivot.z;
	
	return *this;
}




// Map point to coordinate system defined by origin, vx, vy, and vz.
//
//
inline float3 float3::mapped( const float3& origin,
							   const float3& vx,
							   const float3& vy,
							   const float3& vz ) const{
	return getMapped(origin, vx, vy, vz);
}

inline float3 float3::getMapped( const float3& origin,
								  const float3& vx,
								  const float3& vy,
								  const float3& vz ) const
{
	return float3( origin.x + x*vx.x + y*vy.x + z*vz.x,
				   origin.y + x*vx.y + y*vy.y + z*vz.y,
				   origin.z + x*vx.z + y*vy.z + z*vz.z );
}

inline float3& float3::map( const float3& origin,
							 const float3& vx,
							 const float3& vy,
							 const float3& vz )
{
	float xmap = origin.x + x*vx.x + y*vy.x + z*vz.x;
	float ymap =  origin.y + x*vx.y + y*vy.y + z*vz.y;
	z = origin.z + x*vx.z + y*vy.z + z*vz.z;
	x = xmap;
	y = ymap;
	return *this;
}


// Distance between two points.
//
//
inline float float3::distance( const float3& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	return (float)sqrt(vx*vx + vy*vy + vz*vz);
}

inline float  float3::distanceSquared( const float3& pnt ) const{
	return squareDistance(pnt);
}

inline float  float3::squareDistance( const float3& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	return vx*vx + vy*vy + vz*vz;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */

inline float3 float3::interpolated( const float3& pnt, float p ) const {
	return getInterpolated(pnt,p);
}

inline float3 float3::getInterpolated( const float3& pnt, float p ) const {
	return float3( x*(1-p) + pnt.x*p,
				   y*(1-p) + pnt.y*p,
				   z*(1-p) + pnt.z*p );
}

inline float3& float3::interpolate( const float3& pnt, float p ) {
	x = x*(1-p) + pnt.x*p;
	y = y*(1-p) + pnt.y*p;
	z = z*(1-p) + pnt.z*p;
	return *this;
}


inline float3 float3::middled( const float3& pnt ) const {
	return getMiddle(pnt);
}

inline float3 float3::getMiddle( const float3& pnt ) const {
	return float3( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f, (z+pnt.z)/2.0f );
}

inline float3& float3::middle( const float3& pnt ) {
	x = (x+pnt.x)/2.0f;
	y = (y+pnt.y)/2.0f;
	z = (z+pnt.z)/2.0f;
	return *this;
}


// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
//
//
inline float3& float3::average( const float3* points, int num ) {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	for( int i=0; i<num; i++) {
		x += points[i].x;
		y += points[i].y;
		z += points[i].z;
	}
	x /= num;
	y /= num;
	z /= num;
	return *this;
}



// Normalization
//
//
inline float3 float3::normalized() const {
	return getNormalized();
}

inline float3 float3::getNormalized() const {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		return float3( x/length, y/length, z/length );
	} else {
		return float3();
	}
}

inline float3& float3::normalize() {
	float length = (float)sqrt(x*x + y*y + z*z);
	if( length > 0 ) {
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}



// Limit length.
//
//

inline float3 float3::limited(float max) const {
	return getLimited(max);
}

inline float3 float3::getLimited(float max) const {
    float3 limited;
    float lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio);
    } else {
        limited.set(x,y,z);
    }
    return limited;
}

inline float3& float3::limit(float max) {
    float lengthSquared = (x*x + y*y + z*z);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        float ratio = max/(float)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
    }
    return *this;
}


// Perpendicular vector.
//
//
inline float3 float3::crossed( const float3& vec ) const {
	return getCrossed(vec);
}
inline float3 float3::getCrossed( const float3& vec ) const {
	return float3( y*vec.z - z*vec.y,
				   z*vec.x - x*vec.z,
				   x*vec.y - y*vec.x );
}

inline float3& float3::cross( const float3& vec ) {
	float _x = y*vec.z - z*vec.y;
	float _y = z*vec.x - x*vec.z;
	z = x*vec.y - y*vec.x;
	x = _x;
	y = _y;
	return *this;
}

/**
 * Normalized perpendicular.
 */
inline float3 float3::perpendiculared( const float3& vec ) const {
	return getPerpendicular(vec);
}

inline float3 float3::getPerpendicular( const float3& vec ) const {
	float crossX = y*vec.z - z*vec.y;
	float crossY = z*vec.x - x*vec.z;
	float crossZ = x*vec.y - y*vec.x;
	
	float length = (float)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 )
		return float3( crossX/length, crossY/length, crossZ/length );
	else
		return float3();
}

inline float3& float3::perpendicular( const float3& vec ) {
	float crossX = y*vec.z - z*vec.y;
	float crossY = z*vec.x - x*vec.z;
	float crossZ = x*vec.y - y*vec.x;
	
	float length = (float)sqrt(crossX*crossX +
							   crossY*crossY +
							   crossZ*crossZ);
	
	if( length > 0 ) {
		x = crossX/length;
		y = crossY/length;
		z = crossZ/length;
	} else {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	
	return *this;
}


// Length
//
//
inline float float3::length() const {
	return (float)sqrt( x*x + y*y + z*z );
}

inline float float3::lengthSquared() const {
	return (float)(x*x + y*y + z*z);
}

//inline float float3::squareLength() const {
//	return lengthSquared();
//}



/**
 * Angle (deg) between two vectors.
 * This is an unsigned relative angle from 0 to 180.
 * http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
 */
inline float float3::angle( const float3& vec ) const {
	float3 n1 = this->normalized();
	float3 n2 = vec.normalized();
	return (float)(acos( n1.dot(n2) )*RAD_TO_DEG);
}

inline float float3::angleRad( const float3& vec ) const {
	float3 n1 = this->normalized();
	float3 n2 = vec.normalized();
	return (float)acos( n1.dot(n2) );
}



/**
 * Dot Product.
 */
inline float float3::dot( const float3& vec ) const {
	return x*vec.x + y*vec.y + z*vec.z;
}





// Non-Member operators
//
//
inline float3 operator+( float f, const float3& vec ) {
    return float3( f+vec.x, f+vec.y, f+vec.z );
}

inline float3 operator-( float f, const float3& vec ) {
    return float3( f-vec.x, f-vec.y, f-vec.z );
}

inline float3 operator*( float f, const float3& vec ) {
    return float3( f*vec.x, f*vec.y, f*vec.z );
}

inline float3 operator/( float f, const float3& vec ) {
    return float3( f/vec.x, f/vec.y, f/vec.z);
}
