#pragma once

//Целочисленный прямоугольник

#include "kuConstants.h"
#include "int2.h"
#include "float2.h"
//#include "ofLog.h"

class kuRectf;

class kuRecti {
	
public:

    kuRecti();
    virtual ~kuRecti();
    kuRecti(int px, int py, int w, int h);
    kuRecti(const int2& p, int w, int h);
    kuRecti(const kuRecti& rect);
    kuRecti(const int2& p0, const int2& p1);
    //kuRecti( kuRectf r ); { x = r.x; y = r.y; w = r.w; h = r.h; }

    void set(int px, int py, int w, int h);
    void set(const int2& p, int w, int h);
    void set(const kuRecti& rect);
    void set(const int2& p0, const int2& p1);
    void setFromPoints( int x0, int y0, int x1, int y1 );

    void setX(int px);
    void setY(int py);
    
    void setWidth(int w);
    void setHeight(int h);

    void setPosition(int px, int py);
    void setPosition(const int2& p);
    
    void setFromCenter(int px, int py, int w, int h);
    void setFromCenter(const int2& p, int w, int h);

    void translate(int dx, int dy);
    void translate(const int2& dp);
    
    void translateX(int dx);
    void translateY(int dy);

    void scale(int s);
    void scale(int sX, int sY);
    void scale(const int2& s);
    
    void scaleWidth(int  sX);
    void scaleHeight(int sY);
    
    void scaleFromCenter(int s);
    void scaleFromCenter(int sX, int sY);
    void scaleFromCenter(const int2& s);
    
    bool isCorrect();   //w>=0, h>=0
    bool inside(int px, int py) const;
    bool inside(const int2& p) const;
    bool inside(const kuRecti& rect) const;
    bool inside(const int2& p0, const int2& p1) const;
    bool isInside( const kuRecti& rect ) const;


    bool intersects(const kuRecti& rect) const;
    //bool intersects(const int2& p0, const int2& p1) const;
    
    void growToInclude(int px, int py);
    void growToInclude(const int2& p);
    void growToInclude(const kuRecti& rect);
    void growToInclude(const int2& p0, const int2& p1);

    void inflate( int valueX, int valueY );          //раздуть, в каждую сторону на значения

    kuRecti getIntersection(const kuRecti& rect) const;

    kuRecti getUnion(const kuRecti& rect) const;
    
    void standardize();
    kuRecti getStandardized() const;
    bool isStandardized() const;  // are w/h >= 0.0f
    
    int getArea() const;
    int getPerimeter() const;
    int getAspectRatio() const;
    
    bool isEmpty() const;     // are w/h == 0.0f
    
    int2 getMin() const;
    int2 getMax() const;
    
    int getMinX() const;
    int getMaxX() const;
    int getMinY() const;
    int getMaxY() const;
    
    int getLeft()   const;
    int getRight()  const;
    int getTop()    const;
    int getBottom() const;
    
    int2 getTopLeft() const;
    int2 getTopRight() const;
    int2 getBottomLeft() const;
    int2 getBottomRight() const;
    
    int2  getPosition() const;
    int2& getPositionRef();

    int2 getCenter() const;
    
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;

    kuRecti& operator = (const kuRecti& rect);
    kuRecti& operator + (const int2& p);
    
    bool operator == (const kuRecti& rect) const;
    bool operator != (const kuRecti& rect) const;
    
    int2 position;
    int& x;
    int& y;
    int w;
    int h;

    int x1() { return x + w; }
    int y1() { return y + h; }
};

class kuRectf {
public:
    kuRectf() { x = y = w = h = 0; }
    kuRectf( float x0, float y0, float w0, float h0 ) {
        x = x0; y = y0; w = w0; h = h0;
    }

    kuRectf( const kuRecti &r ) {
        x = r.x; y = r.y; w = r.w; h = r.h;
    }

    void setFromPoints( float x0, float y0, float x1, float y1 ) {
        x = x0; y = y0; w = x1 - x0; h = y1 - y0;
    }

    float x, y, w, h;
    float x1() { return x + w; }
    float y1() { return y + h; }

    bool intersects( const kuRectf &r ) {
        return max( x, r.x ) <= min( x+w, r.x+r.w )
                && max( y, r.y ) <= min( y+h, r.y+r.h );
    }
    bool intersects( const kuRecti &r ) {
        kuRectf rf( r );
        return intersects( rf );
    }

    bool inside( const float2 &p ) {  //принадлежность точки, включая правую границу!
        return ( p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h );
    }
    bool inside_d( double X, double Y ) {  //принадлежность точки, включая правую границу!
        return ( X >= x && X <= double(x) + w && Y >= y && Y <= double(y) + h );
    }

    bool insideRotated( float x, float y, float ax, float ay ) {
        return inside( float2(x * ax - y * ay, x * ay + y * ax ) );
    }
    bool insideRotated_d( double x, double y, double ax, double ay ) {
        return inside_d( x * ax - y * ay, x * ay + y * ax );
    }

    kuRecti getRecti() { return kuRecti( x, y, w, h ); }

};

class kuRectd {
public:
    kuRectd() { x = y = w = h = 0; }
    kuRectd( double x0, double y0, double w0, double h0 ) {
        x = x0; y = y0; w = w0; h = h0;
    }

    kuRectd( const kuRecti &r ) {
        x = r.x; y = r.y; w = r.w; h = r.h;
    }

    void setFromPoints( double x0, double y0, double x1, double y1, bool normalize0 = false ) {
        x = x0; y = y0; w = x1 - x0; h = y1 - y0;
        if ( normalize0 ) { normalize(); }
    }
    void setFromPointsNormalized( double x0, double y0, double x1, double y1 ) {
        setFromPoints( x0, y0, x1, y1 );
    }

    void normalize() {  //делает чтобы x <= x1, y <= y1
        setFromPoints( min(x,x1()), min(y,y1()), max(x,x1()), max(y,y1()) );
    }

    void setFromString(string line, const string &delim = " ", bool assert = true, bool normalize = false );
    void setFromStringNormalized(string line, const string &delim = " ", bool assert = true );

    string toString( const string &delim = " ") const;

    double x, y, w, h;
    double x1() const { return x + w; }
    double y1() const { return y + h; }

    //пересекаются ли
    bool intersects( const kuRectd &r ) {
        return max( x, r.x ) <= min( x+w, r.x+r.w )
                && max( y, r.y ) <= min( y+h, r.y+r.h );
    }
    //пересекаются с непустой внутренностью
    bool intersectsStrict( const kuRectd &r ) {
        return max( x, r.x ) < min( x+w, r.x+r.w )
                && max( y, r.y ) < min( y+h, r.y+r.h );
    }
    bool intersects( const kuRecti &r ) {
        kuRectd rf( r );
        return intersects( rf );
    }

    //this лежит внутри прямоугольника r
    bool isInsideRect( kuRectd &r ) {
        return x >= r.x && x1() <= r.x1() && y >= r.y && y1() <= r.y1();
    }

    //точка внутри this
    bool inside( double X, double Y, double eps ) {  //принадлежность точки, включая правую границу!
        return ( X >= x-eps && X <= x + w+eps && Y >= y-eps && Y <= y + h+eps );
    }

    bool insideRotated( double x, double y, double ax, double ay, double eps ) {
        return inside( x * ax - y * ay, x * ay + y * ax, eps );
    }

    kuRecti getRecti() { return kuRecti( x, y, w, h ); }

    bool isAlmostEqual( kuRectd r, double eps ) {
        return fabs(x-r.x) <= eps && fabs(y-r.y) <= eps && fabs(x1()-r.x1()) <= eps && fabs(y1()-r.y1()) <= eps;
    }

    //Площадь
    double area() { return w*h; }
    //Прямоугольник пересечения - вызывать только если пересекаются (intersects)
    kuRectd intersect( const kuRectd &r ) {
        kuRectd res;
        res.setFromPoints( max( x, r.x ),  max( y, r.y ),
                           min( x1(), r.x1() ), min( y1(), r.y1() ) );
        return res;
    }



};

//Габаритный прямоугольник
kuRecti kuBoundaryBox( const vector<int2> &p );
kuRectf kuBoundaryBox( const vector<float2> &p );

//объединение прямоугольников
kuRectf kuRectUnion( const kuRectf &r1, const kuRectf &r2 );
