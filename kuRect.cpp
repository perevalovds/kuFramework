//#include <cint>
#include "kuRect.h"
#include "kuMain.h"

//----------------------------------------------------------
kuRecti::kuRecti() : x(position.x), y(position.y) {
    set(0,0,0,0);
}

//----------------------------------------------------------
kuRecti::~kuRecti(){}

//----------------------------------------------------------
kuRecti::kuRecti(int px, int py, int w, int h) : x(position.x), y(position.y) {
	set(px,py,w,h);
}

//----------------------------------------------------------
kuRecti::kuRecti(const int2& p, int w, int h) : x(position.x), y(position.y) {
    set(p,w,h);
}

//----------------------------------------------------------
kuRecti::kuRecti(const kuRecti& rect) : x(position.x), y(position.y) {
    set(rect);
}

//----------------------------------------------------------
kuRecti::kuRecti(const int2& p0, const int2& p1) : x(position.x), y(position.y) {
    set(p0,p1);
}

//----------------------------------------------------------
void kuRecti::set(int px, int py, int w0, int h0) {
    x = px;
    y = py;
    w = w0;
    h = h0;
}

//----------------------------------------------------------
void kuRecti::set(const int2& p, int w, int h) {
    set(p.x, p.y, w, h);
}

//----------------------------------------------------------
void kuRecti::set(const kuRecti& rect){
    set(rect.x, rect.y, rect.w, rect.h);
}

//----------------------------------------------------------
void kuRecti::set(const int2& p0, const int2& p1) {
    int x0,y0,x1,y1;
    
    x0 = MIN(p0.x, p1.x);
    x1 = MAX(p0.x, p1.x);
    y0 = MIN(p0.y, p1.y);
    y1 = MAX(p0.y, p1.y);
    
    int w = x1 - x0;
    int h = y1 - y0;

    set(x0,y0,w,h);
}

//----------------------------------------------------------
void kuRecti::setFromPoints( int x0, int y0, int x1, int y1 ) {
    set( int2( x0, y0), int2( x1, y1 ) );
}


//----------------------------------------------------------
void kuRecti::setX(int px) {
    x = px;
}

//----------------------------------------------------------
void kuRecti::setY(int py) {
    y = py;
}

//----------------------------------------------------------
void kuRecti::setWidth(int w0) {
    w = w0;
}

//----------------------------------------------------------
void kuRecti::setHeight(int h0) {
    h = h0;
}

//----------------------------------------------------------
void kuRecti::setPosition(int px, int py) {
    position.x = px;
    position.y = py;
}

//----------------------------------------------------------
void kuRecti::setPosition(const int2& p) {
    position = p;
}

//----------------------------------------------------------
void kuRecti::setFromCenter(int px, int py, int w, int h) {
    set(px - w*0.5f, py - h*0.5f, w, h);
}

//----------------------------------------------------------
void kuRecti::setFromCenter(const int2& p, int w, int h) {
    setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
void kuRecti::translate(int dx, int dy) {
    translateX(dx);
    translateY(dy);
}

//----------------------------------------------------------
void kuRecti::translate(const int2& dp) {
    translateX(dp.x);
    translateY(dp.y);
}

//----------------------------------------------------------
void kuRecti::translateX(int dx) {
    x += dx;
}

//----------------------------------------------------------
void kuRecti::translateY(int dy) {
    y += dy;
}

//----------------------------------------------------------
void kuRecti::scale(int s) {
    scaleWidth(s);
    scaleHeight(s);
}

//----------------------------------------------------------
void kuRecti::scale(int sX, int sY) {
    scaleWidth(sX);
    scaleHeight(sY);
}

//----------------------------------------------------------
void kuRecti::scale(const int2& s) {
    scaleWidth(s.x);
    scaleHeight(s.y);
}

//----------------------------------------------------------
void kuRecti::scaleWidth(int  sX) {
    w  *= sX;
}
//----------------------------------------------------------
void kuRecti::scaleHeight(int sY) {
    h *= sY;
}

//----------------------------------------------------------
void kuRecti::scaleFromCenter(int s) {
    scaleFromCenter(s,s);
}

//----------------------------------------------------------
void kuRecti::scaleFromCenter(int sX, int sY) {
    scaleFromCenter(int2(sX,sY));
}

//----------------------------------------------------------
void kuRecti::scaleFromCenter(const int2& s) {
    if(s.x == 1.0f && s.y == 1.0f) return; // nothing to do
    
    int newWidth  = w  * s.x;
    int newHeight = h * s.y;

    int2 center = getCenter();
    
    x = center.x - newWidth  / 2.0f;
    y = center.y - newHeight / 2.0f; 
    
    w  = newWidth;
    h = newHeight;
}

//----------------------------------------------------------
bool kuRecti::isCorrect() {   //w>=0, h>=0
    return (w >= 0) && (h >= 0);
}

//----------------------------------------------------------
bool kuRecti::inside(int px, int py) const {
    return inside(int2(px,py));
}

//----------------------------------------------------------
bool kuRecti::inside(const int2& p) const {
    return p.x >= x && p.y >= y &&
           p.x < x+w && p.y < y+h;
}

//----------------------------------------------------------
bool kuRecti::inside(const kuRecti& rect) const {
    return inside(rect.x,rect.y) &&
           inside(rect.x+rect.w-1, rect.y+rect.h-1);
}

//----------------------------------------------------------
bool kuRecti::isInside( const kuRecti& rect ) const {
    return rect.inside( *this );
}

//----------------------------------------------------------
bool kuRecti::inside(const int2& p0, const int2& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool kuRecti::intersects(const kuRecti& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
/*bool kuRecti::intersects(const int2& p0, const int2& p1) const {
    // check for a line intersection
    int2 p;
    
    int2 topLeft     = getTopLeft();
    int2 topRight    = getTopRight();
    int2 bottomRight = getBottomRight();
    int2 bottomLeft  = getBottomLeft();
    
    return inside(p0) || // check end inside
           inside(p1) || // check end inside
           ofLineSegmentIntersection(p0, p1, topLeft,     topRight,    p) || // cross top
           ofLineSegmentIntersection(p0, p1, topRight,    bottomRight, p) || // cross right
           ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
           ofLineSegmentIntersection(p0, p1, bottomLeft,  topLeft,     p);   // cross left
}*/

//----------------------------------------------------------
void kuRecti::growToInclude(int px, int py) {
    growToInclude(int2(px,py));
}

//----------------------------------------------------------
void kuRecti::growToInclude(const int2& p) {
    int x0 = MIN(getMinX(),p.x);
    int x1 = MAX(getMaxX(),p.x);
    int y0 = MIN(getMinY(),p.y);
    int y1 = MAX(getMaxY(),p.y);
    int w = x1 - x0;
    int h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void kuRecti::growToInclude(const kuRecti& rect) {
    int x0 = MIN(getMinX(),rect.getMinX());
    int x1 = MAX(getMaxX(),rect.getMaxX());
    int y0 = MIN(getMinY(),rect.getMinY());
    int y1 = MAX(getMaxY(),rect.getMaxY());
    int w = x1 - x0;
    int h = y1 - y0;
    set(x0,y0,w,h);
}

//----------------------------------------------------------
void kuRecti::growToInclude(const int2& p0, const int2& p1) {
    growToInclude(p0);
    growToInclude(p1);
}


//----------------------------------------------------------
void kuRecti::inflate( int valueX, int valueY ) {          //раздуть
    x -= valueX;
    w += valueX*2;
    y -= valueY;
    h += valueY*2;
}

//----------------------------------------------------------
kuRecti kuRecti::getIntersection(const kuRecti& rect) const {

    int x0 = MAX(getMinX(),rect.getMinX());
    int x1 = MIN(getMaxX(),rect.getMaxX());
    
    int w = x1 - x0;
    if(w < 0.0f) return kuRecti(0,0,0,0); // short circuit if needed
    
    int y0 = MAX(getMinY(),rect.getMinY());
    int y1 = MIN(getMaxY(),rect.getMaxY());
    
    int h = y1 - y0;
    if(h < 0.0f) return kuRecti(0,0,0,0);  // short circuit if needed
    
    return kuRecti(x0,y0,w,h);
}

//----------------------------------------------------------
kuRecti kuRecti::getUnion(const kuRecti& rect) const {
    kuRecti united = *this;
    united.growToInclude(rect);
    return united;
}

//----------------------------------------------------------
void kuRecti::standardize() {
    if(w < 0.0f) {
        x += w;
        w = -1.0f * w;
    } 
    
    if(h < 0.0f) {
        y += h;
        h = -1.0f * h;
    }
}

//----------------------------------------------------------
kuRecti kuRecti::getStandardized() const {
    if(isStandardized()) {
        return *this;
    } else {
        kuRecti canRect(*this); // copy it
        canRect.standardize();
        return canRect;
    }
}

//----------------------------------------------------------
bool kuRecti::isStandardized() const {
    return w >= 0.0f && h >= 0.0f;
}

//----------------------------------------------------------
int kuRecti::getArea() const {
    return abs(w) * abs(h);
}

//----------------------------------------------------------
int kuRecti::getPerimeter() const {
    return 2.0f * abs(w) + 2.0f * abs(h);
}

//----------------------------------------------------------
int kuRecti::getAspectRatio() const {
    return abs(w) / abs(h);
}

//----------------------------------------------------------
bool kuRecti::isEmpty() const {
    return w == 0.0f && h == 0.0f;
}

//----------------------------------------------------------
int2 kuRecti::getMin() const {
    return int2(getMinX(),getMinY());
}

//----------------------------------------------------------
int2 kuRecti::getMax() const {
    return int2(getMaxX(),getMaxY());
}

//----------------------------------------------------------
int kuRecti::getMinX() const {
    return MIN(x, x + w);  // - w
}

//----------------------------------------------------------
int kuRecti::getMaxX() const {
    return MAX(x, x + w);  // - w
}

//----------------------------------------------------------
int kuRecti::getMinY() const{
    return MIN(y, y + h);  // - h
}

//----------------------------------------------------------
int kuRecti::getMaxY() const {
    return MAX(y, y + h);  // - h
}

//----------------------------------------------------------
int kuRecti::getLeft() const {
    return getMinX();
}

//----------------------------------------------------------
int kuRecti::getRight() const {
    return getMaxX();
}

//----------------------------------------------------------
int kuRecti::getTop() const {
    return getMinY();
}

//----------------------------------------------------------
int kuRecti::getBottom() const {
    return getMaxY();
}

//----------------------------------------------------------
int2 kuRecti::getTopLeft() const {
    return getMin();
}

//----------------------------------------------------------
int2 kuRecti::getTopRight() const {
    return int2(getRight(),getTop());
}

//----------------------------------------------------------
int2 kuRecti::getBottomLeft() const {
    return int2(getLeft(),getBottom());
}

//----------------------------------------------------------
int2 kuRecti::getBottomRight() const {
    return getMax();
}

//----------------------------------------------------------
bool kuRecti::operator != (const kuRecti& rect) const {
    return (x != rect.x) || (y != rect.y) || (w != rect.w) || (h != rect.h);
}

//----------------------------------------------------------
int2 kuRecti::getPosition() const {
    return position;
}

//----------------------------------------------------------
int2& kuRecti::getPositionRef() {
    return position;
}

//----------------------------------------------------------
int2 kuRecti::getCenter() const {
    return int2(x + w * 0.5f, y + h * 0.5f);
}

//----------------------------------------------------------
int kuRecti::getX() const {
    return x;
}

//----------------------------------------------------------
int kuRecti::getY() const {
    return y;
}

//----------------------------------------------------------
int kuRecti::getWidth() const {
    return w;
}

//----------------------------------------------------------
int kuRecti::getHeight() const {
    return h;
}

//----------------------------------------------------------
kuRecti& kuRecti::operator = (const kuRecti& rect) {
    set(rect);
	return *this;
}

//----------------------------------------------------------
kuRecti& kuRecti::operator + (const int2 & point){
	x += point.x;
	y += point.y;
	return *this;
}

//----------------------------------------------------------
bool kuRecti::operator == (const kuRecti& rect) const {
    return (x == rect.x) && (y == rect.y) && (w == rect.w) && (h == rect.h);
}

//----------------------------------------------------------
kuRecti kuBoundaryBox( const vector<int2> &p ) {
    int x0 = 0;
    int y0 = 0;
    int x1 = -1;
    int y1 = -1;
    if ( !p.empty() ) {
        x0 = x1 = p[0].x;
        y0 = y1 = p[0].y;
        for (size_t i=1; i<p.size(); i++) {
            const int2 &q = p[i];
            x0 = min( x0, q.x );
            y0 = min( y0, q.y );
            x1 = max( x1, q.x );
            y1 = max( y1, q.y );
        }
    }
    return kuRecti( x0, y0, x1-x0+1, y1-y0+1);
}

//----------------------------------------------------------
kuRectf kuBoundaryBox( const vector<float2> &p ) {
    float x0 = 0;
    float y0 = 0;
    float x1 = 0;
    float y1 = 0;
    if ( !p.empty() ) {
        x0 = x1 = p[0].x;
        y0 = y1 = p[0].y;
        for (size_t i=1; i<p.size(); i++) {
            const float2 &q = p[i];
            x0 = min( x0, q.x );
            y0 = min( y0, q.y );
            x1 = max( x1, q.x );
            y1 = max( y1, q.y );
        }
    }
    return kuRectf( x0, y0, x1-x0, y1-y0);
}

//----------------------------------------------------------
kuRectf kuRectUnion( const kuRectf &r1, const kuRectf &r2 ) {
    float x0, y0, x1, y1;
    x0 = min( r1.x, r1.x + r1.w );
    x0 = min( x0, min( r2.x, r2.x + r2.w ) );
    x1 = max( r1.x, r1.x + r1.w );
    x1 = max( x1, max( r2.x, r2.x + r2.w ) );
    y0 = min( r1.y, r1.y + r1.h );
    y0 = min( y0, min( r2.y, r2.y + r2.h ) );
    y1 = max( r1.y, r1.y + r1.h );
    y1 = max( y1, max( r2.y, r2.y + r2.h ) );
    return kuRectf( x0, y0, x1 - x0, y1 - y0 );
}

//----------------------------------------------------------
void kuRectd::setFromString( string line, const string &delim, bool assert, bool normalize0 ) {
    kuStringTrim( line );
    vector<string> list = kuSplitString( line, delim );
    kuAssert( list.size() >= 4 || !assert, "cmRectd::setFromString - bad line " + line );
    setFromPoints( kuToDouble(list[0]), kuToDouble(list[1]), kuToDouble(list[2]), kuToDouble(list[3]), normalize0 );
}

//----------------------------------------------------------
void kuRectd::setFromStringNormalized(string line, const string &delim, bool assert ) {
    setFromString( line, delim, assert, true );
}

//----------------------------------------------------------
string kuRectd::toString( const string &delim ) const {
    return kuToString( x, 14 ) + delim + kuToString( y, 14 )
            + delim + kuToString( x1(), 14 ) + delim + kuToString( y1(), 14 );
}

//----------------------------------------------------------
