#include "float2.h"
#include "float3.h"
//#include "ofVec4f.h"

float2::float2( const float3& vec ) {
	x = vec.x;
	y = vec.y;
}

//float2::float2( const ofVec4f& vec ) {
//	x = vec.x;
//	y = vec.y;
//}

//поиск разложения Z = aX + bY
void decompose( const float2 &Z, const float2 &X, const float2 &Y, float &a, float &b, bool &valid ) {
    float d = X.x * Y.y - X.y * Y.x;
    valid = (fabs(d) > 0.000001);   //TODO EPS
    if ( valid ) {
        float d1 = Z.x * Y.y - Z.y * Y.x;
        float d2 = X.x * Z.y - X.y * Z.x;
        a = d1 / d;
        b = d2 / d;
    }
    else {
        a = b = 0;
    }
}


