#include "kuMath.h"
//#include "ofUtils.h"
//#include "ofAppRunner.h"
#include "float.h"		//#define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */

//#include "ofPolyline.h"

//--------------------------------------------------
int kuNextPow2(int a){
	// from nehe.gamedev.net lesson 43
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

//--------------------------------------------------
void kuSeedRandom() {

	// good info here:
	// http://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand
/*
	#ifdef TARGET_WIN32
		srand(GetTickCount());
	#else
		// use XOR'd second, microsecond precision AND pid as seed
		struct timeval tv;
		gettimeofday(&tv, 0);
		long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
		srand(n);
    #endif*/
}

//--------------------------------------------------
void kuSeedRandom(int val) {
	srand((long) val);
}

//--------------------------------------------------
float kuRandom(float max) {
	return max * rand() / (RAND_MAX + 1.0f);
}

//--------------------------------------------------
float kuRandom(float x, float y) {

	float high = 0;
	float low = 0;
	float randNum = 0;
	// if there is no range, return the value
	if (x == y) return x; 			// float == ?, wise? epsilon?
	high = MAX(x,y);
	low = MIN(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}

//--------------------------------------------------
float kuRandomf() {
	float randNum = 0;
	randNum = (rand()/(RAND_MAX + 1.0)) * 2.0 - 1.0;
	return randNum;
}

//--------------------------------------------------
float kuRandomuf() {
	float randNum = 0;
	randNum = rand()/(RAND_MAX + 1.0);
	return randNum;
}

//--------------------------------------------------
int kuRound( float value ) {
    return ( value >= 0 ) ? int( value + 0.5 ) : -( int(-value+0.5) );
}

//--------------------------------------------------
float kuNormalize(float value, float min, float max){
	return kuClamp( (value - min) / (max - min), 0, 1);
}

//--------------------------------------------------
float kuMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < FLT_EPSILON){
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	
		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}
}

//--------------------------------------------------
ldouble kuMapldouble(ldouble value, ldouble inputMin, ldouble inputMax, ldouble outputMin, ldouble outputMax, bool clamp) {
    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        return outputMin;
    } else {
        ldouble outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
}


//--------------------------------------------------
float kuDist(float x1, float y1, float x2, float y2) {
	return sqrt(double((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

//--------------------------------------------------
float kuDistSquared(float x1, float y1, float x2, float y2) {
	return ( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

//--------------------------------------------------
float kuClamp(float value, float min, float max) {
	return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------
int kuClampi(int value, int min, int max) {
    return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------
int kuSign(float n) {
	if( n > 0 ) return 1;
	else if(n < 0) return -1;
	else return 0;
}

//--------------------------------------------------
bool kuInRange(float t, float min, float max) {
	return t>=min && t<=max;
}

//--------------------------------------------------------------
bool kuInRangeExi( int x, int a, int bExcluded ) //a <= x < bExcluded
{
    return (a <= x && x < bExcluded );
}

//--------------------------------------------------------------
bool kuInRangei( int x, int a, int b ) //a <= x <= b
{
    return (a <= x && x <= b );
}


//--------------------------------------------------
bool kuInAreai( int x, int y, int w, int h ) {
    return ( x >= 0 && x < w && y >= 0 && y < h );
}

//--------------------------------------------------
float kuRadToDeg(float radians) {
	return radians * RAD_TO_DEG;
}

//--------------------------------------------------
float kuDegToRad(float degrees) {
    return degrees * DEG_TO_RAD;
}

//--------------------------------------------------
float kuLerp(float start, float stop, float amt) {
	return start + (stop-start) * amt;
}

//--------------------------------------------------
float kuWrapRadians(float angle, float from, float to){
	while (angle > to ) angle -= TWO_PI;
	while (angle < from ) angle += TWO_PI;
	return angle;
}


float kuWrapDegrees(float angle, float from, float to){
	while (angle > to ) angle-=360;
	while (angle < from ) angle+=360;
	return angle;

}

//--------------------------------------------------
float kuLerpDegrees(float currentAngle, float targetAngle, float pct) {
    return currentAngle + kuAngleDifferenceDegrees(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float kuLerpRadians(float currentAngle, float targetAngle, float pct) {
	return currentAngle + kuAngleDifferenceRadians(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float kuAngleDifferenceDegrees(float currentAngle, float targetAngle) {
	return kuWrapDegrees(targetAngle - currentAngle);
}

//--------------------------------------------------
float kuAngleDifferenceRadians(float currentAngle, float targetAngle) {
	return  kuWrapRadians(targetAngle - currentAngle);
}


/*
//--------------------------------------------------
bool ofInsidePoly(float x, float y, const vector<ofPoint> & polygon){
    return ofPolyline::inside(x,y, ofPolyline(polygon));
}

//--------------------------------------------------
bool ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly){
    return ofPolyline::inside(p.x,p.y, ofPolyline(poly));
}

//--------------------------------------------------
bool ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End, ofPoint & intersection){
	ofPoint diffLA, diffLB;
	float compareA, compareB;
	diffLA = line1End - line1Start;
	diffLB = line2End - line2Start;
	compareA = diffLA.x*line1Start.y - diffLA.y*line1Start.x;
	compareB = diffLB.x*line2Start.y - diffLB.y*line2Start.x;
	if (
		(
			( ( diffLA.x*line2Start.y - diffLA.y*line2Start.x ) < compareA ) ^
			( ( diffLA.x*line2End.y - diffLA.y*line2End.x ) < compareA )
		)
		&&
		(
			( ( diffLB.x*line1Start.y - diffLB.y*line1Start.x ) < compareB ) ^
			( ( diffLB.x*line1End.y - diffLB.y*line1End.x) < compareB )
		)
	)
	{
		float lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
		intersection.x =  -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv ;
		intersection.y =  -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv ;

		return true;
	}

	return false;
}
*/
//--------------------------------------------------
