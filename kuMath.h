#pragma once

//#include "ofPoint.h"
#include "kuConstants.h"

#define kuSqr(x) (x * x)
int 		kuNextPow2 ( int a );
void 		kuSeedRandom();
void 		kuSeedRandom(int val);
float		kuRandom(float max); // random (0 - max)
float 		kuRandom(float val0, float val1);		// random (x - y)
float 		kuRandomf();							// random (-1 - 1)
float 		kuRandomuf();							// random (0 - 1) 

int         kuRound( float value );

float		kuNormalize(float value, float min, float max);
float		kuMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
ldouble		kuMapldouble(ldouble value, ldouble inputMin, ldouble inputMax, ldouble outputMin, ldouble outputMax, bool clamp = false);
float		kuClamp(float value, float min, float max);
int 		kuClampi(int value, int min, int max);

float		kuLerp(float start, float stop, float amt);
float		kuDist(float x1, float y1, float x2, float y2);
float		kuDistSquared(float x1, float y1, float x2, float y2);
int			kuSign(float n);
bool		kuInRange(float t, float min, float max);
bool        kuInRangeExi( int x, int a, int bExcluded ); //a <= x < bExcluded
bool        kuInRangei( int x, int a, int b ); //a <= x <= b
bool        kuInAreai( int x, int y, int w, int h );


float		kuRadToDeg(float radians);
float		kuDegToRad(float degrees);
float 		kuLerpDegrees(float currentAngle, float targetAngle, float pct);
float 		kuLerpRadians(float currentAngle, float targetAngle, float pct);
float 		kuAngleDifferenceDegrees(float currentAngle, float targetAngle);
float 		kuAngleDifferenceRadians(float currentAngle, float targetAngle);
float 		kuAngleSumRadians(float currentAngle, float targetAngle);
float		kuWrapRadians(float angle, float from = -PI, float to=+PI);
float		kuWrapDegrees(float angle, float from = -180, float to=+180);

//bool        ofInsidePoly(float x, float y, const vector<ofPoint> & poly);
//bool        ofInsidePoly(const ofPoint & p, const vector<ofPoint> & poly);

//bool 		ofLineSegmentIntersection(ofPoint line1Start, ofPoint line1End, ofPoint line2Start, ofPoint line2End, ofPoint & intersection);


