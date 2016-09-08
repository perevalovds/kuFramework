#pragma once

//Растр
#include <limits>       // std::numeric_limits
#include "kuConstants.h"
#include "kuUtils.h"
#include "kuLog.h"
#include "kuMath.h"
#include "kuPoint.h"
#include "kuRect.h"

const int KU_INTERP_NEAREST = 0;        //ближайший сосед
const int KU_INTERP_BILINEAR = 1;
const int KU_INTERP_BICUBIC = 2;
const int KU_INTERP_SAMPLEAREA = 3;     //лучший способ, но реализуется самими алгоритмами

template <typename T> class kuRaster_;

struct u8_rgb {
    u8 r, g, b;
};
struct u8_v3 {
    u8 v[3];
};

union u8_3 {
    u8_rgb c;
    u8_v3 v;
    u8_3& operator=(int val) {
        c.r=c.g=c.b=val;
        return *this;
    }
    u8_3( int val = 0 ) {
        c.r=c.g=c.b=val;
    }
    u8_3( u8 r, u8 g, u8 b ) {
        c.r=r; c.g=g; c.b=b;
    }

};

//Типы растров
typedef kuRaster_<u8> kuRaster_u8;
typedef kuRaster_<s16> kuRaster_s16;
typedef kuRaster_<s32> kuRaster_s32;
typedef kuRaster_<f32> kuRaster_f32;
typedef kuRaster_<u16> kuRaster_u16;
typedef kuRaster_<u8_3> kuRaster_u8_3;		//для его resize - следует использовать kuRasterResize(...)


//Номера типов
const int kuRaster_type_undefined = 0;
const int kuRaster_type_u8 = 1;
const int kuRaster_type_s16 = 2;
const int kuRaster_type_s32 = 3;
const int kuRaster_type_f32 = 4;
const int kuRaster_type_u16 = 5;


//Один растр
//ВНИМАНИЕ: Нельзя присваивать один растр другому, вместо этого следует делать a.copyTo(b)
template <typename T>
class kuRaster_ {
public:
	kuRaster_() { w = h = 0; _pixels = 0; }
	~kuRaster_() { clear(); }

    int type();
    int pixelSize() const { return sizeof(T); }

    void allocate( int w, int h, T fillValue = 0 );
    void setTo( T fillValue );
    void setTo( T value, kuRaster_u8 &mask );    //установить в маске
    void setFromPixels( int w, int h, T* dataToCopy );
    void setFromPixelsPointer( int w, int h, vector<T> *pixelsNoCopy );

    void releasePixels();   //освобождает массив пикселей для внешнего использования
    void clear();
	bool empty() { return ( w > 0 && h > h ); }
    bool isEqualTo( T value );    //равен ли константе
    int countValues( T value );   //подсчёт значений, равных value
    void replaceValue( T value, T value1 );
    void getMask( T vmin, T vmax, kuRaster_u8 &outMask );

	bool allocated() { return !empty(); }


    void loadFromRstFile( const string &fileName );	//rst-файл
    void saveToRstFile( const string &fileName );		//rst-файл

    size_t fileSize();
    void saveToFILE( FILE *f );
    void loadFromFILE( FILE *f );
    void loadBodyFromFILE( FILE *f, int W, int H );

    size_t w, h;

	vector<T> &pixels() { return *_pixels; }
	T* pixelsPointer() { return (empty())? 0 : &pixels()[0]; }
    T &pixel( size_t x, size_t y ) { return pixels()[ x + w * y ]; }  //взятие пикселя без контроля границ
    T &pixelSafe( size_t x, size_t y ) {   //взятие пикселя с контролем границ
        kuAssert( kuInAreai(x,y,w,h), "kuRaster_::pixelSafe - bad pixel " + kuToString(x) + "," + kuToString(y)
                  + ", image " + kuToString(w) + " x " + kuToString(h) );
        return pixels()[ x + w * y ];
    }

    T &pixel( const int2 &point ) { return pixels()[ size_t(point.x) + w * size_t(point.y) ]; }  //взятие пикселя без контроля границ
    void setPixelSafe( float2 p, T value );

    T* pixelsPointer() const { return (empty())? 0 : &pixels()[0]; }
    size_t pixelsPointerSizeInBytes() { return (empty())? 0: size_t(w)*size_t(h)*size_t(pixelSize()); }

    T pixelBilinear( float x, float y );   //билинейная интерполяция
    T pixelBicubic(float x, float y );    //бикубическая интерполяция
    T pixelInterp( float x, float y, int interpType ); //ku_INTERP_NEAREST, ku_INTERP_BILINEAR

	T minValue();
	T maxValue();

    void shrinkTo( kuRaster_<T> &rst, int shrink );
    void resizeTo( kuRaster_<T> &rst, int w1, int h1, int interpType );

    void cropTo( kuRaster_<T> &rst, int x, int y, int w, int h );
    void cropTo( kuRaster_<T> &rst, const kuRecti &r ) { cropTo( rst, r.x, r.y, r.w, r.h ); }

    void draw( kuRaster_<T> &rst, int X, int Y );

    void pasteTo( kuRaster_<T> &rst, int X, int Y ) { draw( rst, X, Y ); }

    template <typename T1>
    void copyTo( kuRaster_<T1> &rst );
    template <typename T1>
    void copyToNormalized( kuRaster_<T1> &rst, double minV, double maxV ); //нормализовать значения

    void normalize(double minV, double maxV , double gamma); //нормализовать значения, с гамма-коррекцией

    template <typename T1>
    bool equalSize( kuRaster_<T1> &rst ) { return (w == rst.w && h == rst.h); }   //одинаковый размер

    void shift( int x, int y, T fillValue = 0 );
    //фильтр - min, median, по круговой окрестности или отрезку, фильтрует только в точках маски
    void filterAtMask( kuRaster_<T> &rstOut, kuRaster_<u8> &mask0,
                        u8 maskVal, int maskIgnore,
                       int rad,
                       const string &filterType, bool useAngle = false, float angle = 0 );

    //поворот, масштабирование, перенос, по двум точкам
    void warp_orthoTo( kuRaster_<T> &rst, int outW, int outH,
                       const vector<float2> &src, const vector<float2> &dst );

    void threshold( T thresh, T value0, T value1 );
private:
    vector<T> *_pixels;

	
	// copy and assignment prohibited	
	//https://habrahabr.ru/company/abbyy/blog/142595/
	kuRaster_( const kuRaster_& );
	void kuRaster_::operator=( const kuRaster_& );
};



//Заголовок файла
static const int kuRasterHeader_versionLen = 8;         //версия формата
static const int kuRasterHeader_typeLen = 8;            //тип

struct kuRasterHeader {
    u8 _version[ kuRasterHeader_versionLen ];
    u8 _type[ kuRasterHeader_typeLen ];
    u32 pixelSize;
    u32 w, h;

    kuRasterHeader();
    kuRasterHeader( const string &version, int type, int pixelSize, int w, int h );
    void saveToFILE( FILE *f );
    void loadFromFILE( FILE *f );
    void loadFromRstFile( const string &fileName );	//rst-файл
    string version();
    int type();
    void setVersion( string v );
    void setType( int t );
};


//----------------------------------------
//----------------------------------------
//----------------------------------------

template <typename T>
void kuRaster_<T>::allocate( int w0, int h0, T fillValue )
{
    kuAssert( w0 >= 0 && h0 >= 0, "kuRaster_<T>::allocate bad size, " + kuToString(w0) + " x " + kuToString(h0) );
	w = w0;
	h = h0;
	if ( _pixels ) {
        if ( _pixels->size() != size_t(w) * size_t(h) ) {
			delete _pixels;				//удаляем если сейчас надо другого размера
			_pixels = 0;
		}
	}
	if ( !_pixels ) {
		_pixels = new vector<T>();
        _pixels->resize( size_t(w) * size_t(h) );
	}
    setTo( fillValue );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::setTo( T fillValue ) {
    fill( _pixels->begin(), _pixels->end(), fillValue );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::setTo( T value, kuRaster_u8 &mask ) {    //установить в маске
    kuAssert( this->equalSize(mask), "kuRaster_<T>::setTo, bad mask size" );
    T *pix = pixelsPointer();
    u8 *mpix = mask.pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        if ( mpix[i] > 0 ) { pix[i] = value; }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::setFromPixels( int w, int h, T* dataToCopy )
{
    allocate( w, h, 0 );
    memcpy( pixelsPointer(), dataToCopy, w * h * sizeof( T ) );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::setFromPixelsPointer( int w0, int h0, vector<T> *pixelsNoCopy ) {
    w = w0;
    h = h0;
    if ( _pixels ) {
        delete _pixels;
    }
    _pixels = pixelsNoCopy;
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::saveToRstFile( const string &fileName )
{
    kuLog << "\tsaving RST " << fileName << " " << w << " " << h << endl;
	//RST, размер пикселя в байтах (s32), w (s32), h (s32), данные
	FILE *f = fopen( fileName.c_str(), "wb" );
    kuAssert( f != 0, "saveToRstFile " + fileName );
    saveToFILE( f );
    fclose( f );

    kuAssert( kuFileExists(fileName), "kuRaster_<T>::saveToRstFile - error writing file " + fileName );

}

//----------------------------------------
template <typename T>
size_t kuRaster_<T>::fileSize() {
    return sizeof( kuRasterHeader ) + size_t(sizeof( T )) * w * h;  //TODO Исправить!!! - на больших файлах не работает
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::saveToFILE( FILE *f ) {
    //kuLog << "\tsaving RST..." << endl;
    kuRasterHeader header( "rst1.1", type(), sizeof(T), w, h );
    header.saveToFILE( f );
    //запись по блокам
    size_t size = size_t(sizeof( T )) * w * h;
    kuFileSaveArray( f, pixelsPointer(), size );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::loadFromFILE( FILE *f ) {
    //kuLog << "\tloading RST..." << endl;

    kuRasterHeader header;// "rst1.1", type(), sizeof(T), w, h );
    header.loadFromFILE( f );
    kuAssert( header.version() == "rst1.1", "loadFromFILE raster format version" );

    kuAssert( header.pixelSize == sizeof( T ) && header.w > 0 && header.h > 0,
              "kuRaster_<T>::loadFromFILE" );

    //kuLog << "\t" << header.w << " x " << header.h << endl;
    loadBodyFromFILE( f, header.w, header.h );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::loadBodyFromFILE( FILE *f, int W, int H ) {
    allocate( W, H, 0 );
    //fread( pixelsPointer(), sizeof( T ), W * H, f );
    size_t size = size_t(sizeof( T )) * w * h;
    kuFileLoadArray( f, pixelsPointer(), size );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::loadFromRstFile( const string &fileName )
{
    kuLog << "\tloading RST " << fileName << endl;
    kuAssert( kuFileExists( fileName ), "loadFromRstFile, file doesn't exists '" + fileName + "'" );

	FILE *f = fopen( fileName.c_str(), "rb" );
    kuAssert( f != 0, "loadFromRstFile error reading " + fileName );
    loadFromFILE( f );
    fclose( f );
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::clear()									
{
	w = h = 0;
	if ( _pixels ) { delete _pixels; }
	_pixels = 0;
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::releasePixels() {   //освобождает массив пикселей
    w = h = 0;
    _pixels = 0;
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::setPixelSafe(float2 p, T value ) {
    int X = int(p.x);
    int Y = int(p.y);
    if ( kuInAreai(X, Y, w, h) ) {
        pixel(X,Y) = value;
    }
}

//----------------------------------------
template <typename T>
T kuRaster_<T>::pixelBilinear( float x, float y ) {   //билинейная интерполяция
    if ( w == 0 || h == 0 ) { return 0; }
    else {
        int x0 = int(x);
        int y0 = int(y);
        int x1 = x0 + 1;
        int y1 = y0 + 1;
        x0 = kuClampi(x0, 0, w-1);
        y0 = kuClampi(y0, 0, h-1);
        x1 = kuClampi(x1, 0, w-1);
        y1 = kuClampi(y1, 0, h-1);
        float a = kuClamp( x - x0, 0, 1 );
        float b = kuClamp( y - y0, 0, 1 );

        if ( fabs( x - x0 ) <= 1 && fabs( y - y0 ) <= 1) {
            return pixel( x0, y0 ) * (1-a) * (1-b)
                    + pixel( x1, y0 ) * (a) * (1-b)
                    + pixel( x1, y1 ) * (a) * (b)
                    + pixel( x0, y1 ) * (1-a) * (b);
        }
        else return 0;
    }
}

//----------------------------------------
template <typename T>
T kuRaster_<T>::pixelBicubic(float xs, float ys) {    //бикубическая интерполяция
    if ( w == 0 || h == 0 ) { return 0; }
    else {
        int xc = int( xs );
        int yc = int( ys );
        if ( !kuInAreai( xc, yc, w, h ) ){ return 0; }
        xs -= xc; ys -= yc;
        T p[4][4] = {{0}};
        for ( int j = 0; j < 4; j++ ){
            for ( int i = 0; i < 4; i++ ){
                if ( kuInAreai( i + xc - 1, j + yc - 1, w, h ) ) {
                    p[i][j] = pixel( i + xc - 1, j + yc - 1 );
                }
            }
        }

        double a00 = p[1][1];
        double a01 = -0.5 * p[1][0] + 0.5 * p[1][2];
        double a02 = p[1][0] - 2.5 * p[1][1] + 2.0 * p[1][2] - 0.5 * p[1][3];
        double a03 = -0.5 * p[1][0] + 1.5 * p[1][1] - 1.5 * p[1][2] + 0.5 * p[1][3];
        double a10 = -0.5 * p[0][1] + 0.5 * p[2][1];
        double a11 = 0.25 * p[0][0] - 0.25 * p[0][2] - 0.25 * p[2][0] + 0.25 * p[2][2];
        double a12 = -0.5 * p[0][0] + 1.25 * p[0][1] - p[0][2] + 0.25 * p[0][3] + 0.5 * p[2][0] - 1.25 * p[2][1] + p[2][2] - 0.25 * p[2][3];
        double a13 = 0.25 * p[0][0] - 0.75 * p[0][1] + 0.75 * p[0][2] - 0.25 * p[0][3] - 0.25 * p[2][0] + 0.75 * p[2][1] - 0.75 * p[2][2] + 0.25 * p[2][3];
        double a20 = p[0][1] - 2.5 * p[1][1] + 2.0 * p[2][1] - 0.5 * p[3][1];
        double a21 = -0.5 * p[0][0] + 0.5 * p[0][2] + 1.25 * p[1][0] - 1.25 * p[1][2] - p[2][0] + p[2][2] + 0.25 * p[3][0] - 0.25 * p[3][2];
        double a22 = p[0][0] - 2.5 * p[0][1] + 2.0 * p[0][2] - 0.5 * p[0][3] - 2.5 * p[1][0] + 6.25 * p[1][1] - 5.0 * p[1][2] + 1.25 * p[1][3] + 2.0 * p[2][0] - 5.0 * p[2][1] + 4.0 * p[2][2] - p[2][3] - 0.5 * p[3][0] + 1.25 * p[3][1] - p[3][2] + 0.25 * p[3][3];
        double a23 = -0.5 * p[0][0] + 1.5 * p[0][1] - 1.5 * p[0][2] + 0.5 * p[0][3] + 1.25 * p[1][0] - 3.75 * p[1][1] + 3.75 * p[1][2] - 1.25 * p[1][3] - p[2][0] + 3.0 * p[2][1] - 3.0 * p[2][2] + p[2][3] + 0.25 * p[3][0] - 0.75 * p[3][1] + 0.75 * p[3][2] - 0.25 * p[3][3];
        double a30 = -0.5 * p[0][1] + 1.5 * p[1][1] - 1.5 * p[2][1] + 0.5 * p[3][1];
        double a31 = 0.25 * p[0][0] - 0.25 * p[0][2] - 0.75 * p[1][0] + 0.75 * p[1][2] + 0.75 * p[2][0] - 0.75 * p[2][2] - 0.25 * p[3][0] + 0.25 * p[3][2];
        double a32 = -0.5 * p[0][0] + 1.25 * p[0][1] - p[0][2] + 0.25 * p[0][3] + 1.5 * p[1][0] - 3.75 * p[1][1] + 3.0 * p[1][2] - 0.75 * p[1][3] - 1.5 * p[2][0] + 3.75 * p[2][1] - 3.0 * p[2][2] + 0.75 * p[2][3] + 0.5 * p[3][0] - 1.25 * p[3][1] + p[3][2] - 0.25 * p[3][3];
        double a33 = 0.25 * p[0][0] - 0.75 * p[0][1] + 0.75 * p[0][2] - 0.25 * p[0][3] - 0.75 * p[1][0] + 2.25 * p[1][1] - 2.25 * p[1][2] + 0.75 * p[1][3] + 0.75 * p[2][0] - 2.25 * p[2][1] + 2.25 * p[2][2] - 0.75 * p[2][3] - 0.25 * p[3][0] + 0.75 * p[3][1] - 0.75 * p[3][2] + 0.25 * p[3][3];

        double res= ( a00 + a01 * ys + a02 * ys * ys + a03 * ys * ys * ys +
                                a10 * xs + a11 * xs * ys + a12 * xs * ys * ys + a13 * xs * ys * ys * ys +
                                a20 * xs * xs + a21 * xs * xs * ys + a22 * xs * xs * ys * ys + a23 * xs * xs * ys * ys * ys +
                                a30 * xs * xs * xs + a31 * xs * xs * xs * ys + a32 * xs * xs * xs * ys * ys + a33 * xs * xs * xs * ys * ys * ys );
        return  CLAMP( res, numeric_limits<T>::min(), numeric_limits<T>::max() );
    }
}

//----------------------------------------
template <typename T>
T kuRaster_<T>::pixelInterp(float x, float y, int interp ) {   //интерполяция
    if ( interp == KU_INTERP_NEAREST ) {
        int xi = kuClampi( int(x), 0, w-1 );
        int yi = kuClampi( int(y), 0, h-1 );
        return pixel( xi, yi );
    }
    if ( interp == KU_INTERP_BILINEAR ) {
        return pixelBilinear( x, y );
    }
    if ( interp == KU_INTERP_BICUBIC ) {
        return pixelBicubic( x, y );
    }
    kuAssert( false, "kuRaster_<T>::pixelInterp: Interpolation method " + kuToString(interp) + " is not implemented" );
    return 0;
}

//----------------------------------------
/*
//TODO константа
#define PixelInterp_EPS (0.0001)

template <typename T>
T kuRaster_<T>::pixelInterp( float x, float y ) {   //интерполяция
    if ( w == 0 || h == 0 ) { return 0; }
    else {
        int x0 = int(x);
        int y0 = int(y);
        int x1 = x0 + 1;
        int y1 = y0 + 1;
        x0 = kuClampi(x0, 0, w-1);
        y0 = kuClampi(y0, 0, h-1);
        x1 = kuClampi(x1, 0, w-1);
        y1 = kuClampi(y1, 0, h-1);
        float a = kuClamp( x - x0, 0, 1 );
        float b = kuClamp( y - y0, 0, 1 );
        float d1 = kuSqr(a) + kuSqr(b);
        float d2 = kuSqr(1-a) + kuSqr(b);
        float d3 = kuSqr(1-a) + kuSqr(1-b);
        float d4 = kuSqr(a) + kuSqr(1-b);
        float w1,w2,w3,w4;
        if ( d1 < PixelInterp_EPS ) { w1 = 1; w2=w3=w4=0; }
        else {
            if ( d2 < PixelInterp_EPS ) { w2 = 1; w1=w3=w4=0; }
            else {
                if ( d3 < PixelInterp_EPS ) { w3 = 1; w1=w2=w4=0; }
                else {
                    if ( d4 < PixelInterp_EPS ) { w4 = 1; w1=w2=w3=0; }
                    else {
                        d1 = 1/d1;  //алгоритм из wxWidgets, image.cpp
                        d2 = 1/d2;
                        d3 = 1/d3;
                        d4 = 1/d4;

                        float sum = d1+d2+d3+d4;
                        w1 = d1/sum;
                        w2 = d2/sum;
                        w3 = d3/sum;
                        w4 = d4/sum;
                    }
                }
            }

        }
        //w1=1;
        //w2=w3=w4=0;
        return pixel( x0, y0 ) * w1
                + pixel( x1, y0 ) * w2
                + pixel( x1, y1 ) * w3
                + pixel( x0, y1 ) * w4;
    }
}
*/

//----------------------------------------
template <typename T>
T kuRaster_<T>::minValue() {
	if ( w == 0 || h == 0 ) { return 0; }
	else {
		T v = pixels()[0];
        for (size_t i=0; i<w * h; i++) {
			v = min( v, pixels()[ i ] );
		}
		return v;
	}
}

//----------------------------------------
template <typename T>
T kuRaster_<T>::maxValue() {
	if ( w == 0 || h == 0 ) { return 0; }
	else {
		T v = pixels()[0];
        for (size_t i=0; i<w * h; i++) {
			v = max( v, pixels()[ i ] );
		}
		return v;
	}
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::shrinkTo( kuRaster_<T> &rst, int shrink ) {
    if (shrink > 0) {
        size_t w1 = w / shrink;
        size_t h1 = h / shrink;
        cout << w1 << " x " << h1 << endl;
        rst.allocate( w1, h1, 127 );
        if ( w1 > 0 && h1 > 0 ) {
            for (size_t y=0; y<h1; y++) {
                for (size_t x=0; x<w1; x++) {
                    rst.pixel( x, y ) = pixel( x * w / w1, y * h / h1 );
                }
            }
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::resizeTo( kuRaster_<T> &rst, int w1, int h1, int interpType ) {
    rst.allocate( w1, h1, 0 );
    if ( w1 > 0 && h1 > 0 ) {
        for (size_t y=0; y<h1; y++) {
            for (size_t x=0; x<w1; x++) {
                rst.pixel( x, y ) = pixelInterp( double(x) * w / w1, double(y) * h / h1, interpType );
            }
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::cropTo( kuRaster_<T> &rst, int x0, int y0, int w0, int h0 ) {
    kuAssert( kuInRangeExi( x0, 0, w ) && kuInRangeExi( y0, 0, h )
              && w0 >= 0 && h0 >= 0
              && x0+w0 <= w && y0+h0 <= h,
              "kuRaster_<T>::cropTo - incorrect bounds" );
    rst.allocate( w0, h0, 0 );
    for (size_t y=0; y<h0; y++) {
        for (size_t x=0; x<w0; x++) {
            rst.pixel(x,y) = pixel(x+x0, y+y0);
        }
    }
}

//----------------------------------------
template <typename T>
bool kuRaster_<T>::isEqualTo( T value )    //равен ли константе
{
    bool ok = true;
    T *pix = pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        if ( pix[i] != value ) { ok = false; break; }
    }
    return ok;
}

//----------------------------------------
template <typename T>
int kuRaster_<T>::countValues( T value )   //подсчёт значений, равных value
{
    int count = 0;
    T *pix = pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        if ( pix[i] == value ) {
            count++;
        }
    }
    return count;
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::replaceValue( T value, T value1 )
{
    T *pix = pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        if ( pix[i] == value ) {
            pix[i] = value1;
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::getMask( T vmin, T vmax, kuRaster_u8 &outMask ) {
    T *pix = pixelsPointer();
    outMask.allocate( w, h, 0 );
    for (size_t i=0; i<w*h; i++) {
        if ( pix[i] >= vmin && pix[i] <= vmax ) {
            outMask.pixelsPointer()[i] = 255;
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::draw( kuRaster_<T> &rst, int X, int Y ) {
    //TODO оптимизировать - вычислить область которую действительно надо рисовать
    //чтоб не проверять границы в каждом пикселе
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            int x1 = x + X;
            int y1 = y + Y;
            if ( kuInRangeExi( x1, 0, rst.w ) && kuInRangeExi( y1, 0, rst.h ) ) {
                rst.pixel( x1, y1 ) = pixel( x, y );
            }
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::shift( int sx, int sy, T fillValue ) {
    kuLog << "\tshifting raster " << sx << " " << sy << endl;
    int scanx = ( sx > 0 ) ? -1:1;
    int scany = ( sy > 0 ) ? -1:1;
    int x0 = ( scanx > 0 ) ? 0 : (w-1);
    int y0 = ( scany > 0 ) ? 0 : (h-1);

    int y = y0;
    while ( kuInRangeExi( y, 0, h ) ) {
        int x = x0;
        while ( kuInRangeExi( x, 0, w ) ) {
            int y1 = y - sy;
            int x1 = x - sx;
            if ( kuInRangeExi( x1, 0, w ) && kuInRangeExi( y1, 0, h ) ) {
                pixel( x, y ) = pixel( x1, y1 );
            }
            else {
                pixel( x, y ) = fillValue;
            }
            x += scanx;
        }
        y += scany;
    }
    //test
    //for (int y=0; y<h; y++) {
    //    for (int x=0; x<w; x++) {
    //        pixel(x,y) = 0;
    //    }
    //}
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::filterAtMask( kuRaster_<T> &rstOut, kuRaster_u8 &mask,
                    u8 maskVal, int maskIgnore, int rad, const string &filterType, bool useAngle, float angle ) {
    kuLog << "filter at mask" << endl;
    kuLog << "WARNING: crashing if rad > 10" << endl;   //TODO исправить ошибку!!
    kuAssert( rad >= 0, "filterAtMask, bad rad" );

    kuAssert( filterType == "min" || filterType == "median",
              "filterAtMask, unknown filter " + filterType + ", known are min, median");
    bool useMin = (filterType == "min");
    bool useMedian = (filterType == "median");

    //cout << "w " << w << ", h " << h << endl;
    copyTo( rstOut );
    //cout << "rstOut w " << w << ", h " << h << endl;

    vector<int2> pnt;   //окрестность
    if ( !useAngle ) {
        //круг
        for (int y=-rad; y<=rad; y++) {
            for (int x=-rad; x<=rad; x++) {
                if ( x*x + y*y <= rad*rad ) {
                    pnt.push_back( int2( x, y ) );
                }
            }
        }
    }
    else {
        //отрезок
        //TODO нужен алгоритм Брезенхема
        for (int i=-rad; i<=rad; i++) {
            int x = i * cos( angle * DEG_TO_RAD );
            int y = i * sin( angle * DEG_TO_RAD );
            pnt.push_back( int2( x, y ) );
        }
    }


    vector<double> vals;        //TODO перевести на T
    for (int Y=0; Y<h; Y++) {
        if ( Y % 100 == 0 ) { kuLog << Y << "/" << h << endl; }
        for (int X=0; X<w; X++) {
            if ( mask.pixel( X, Y ) == maskVal ) {
                //собираем значения
                vals.clear();
                for (size_t i=0; i<pnt.size(); i++) {
                    int x1 = X + pnt[i].x;
                    int y1 = Y + pnt[i].y;
                    if ( kuInRangeExi( x1, 0, w ) && kuInRangeExi( y1, 0, h )
                         && (mask.pixel( x1, y1 ) != maskVal && mask.pixel( x1, y1 ) != maskIgnore ) ) {
                         //cout << x1 << " " << y1 << endl;
                         vals.push_back( pixel( x1, y1 ) );
                    }
                }
                //фильтруем
                if ( vals.size() > 0 ) {
                    double val = 0;
                    if ( useMin ) {
                        val = vals[0];
                        for (size_t i=0; i<vals.size(); i++) {
                            val = min( val, double(vals[i]) );
                        }
                    }
                    if ( useMedian ) {
                        kuSortDouble( vals );
                        val = vals[ vals.size() / 2 ];
                    }
                    rstOut.pixel( X, Y ) = val;
                }
            }
        }
    }
    //kuLog << endl;
}

//----------------------------------------
//поворот, масштабирование, перенос, по двум точкам
template <typename T>
void kuRaster_<T>::warp_orthoTo( kuRaster_<T> &rst, int W, int H,
                   const vector<float2> &src, const vector<float2> &dst ) {
    kuLog << "warp_orthoTo" << endl;
    kuAssert( w > 0 && h > 0, "warp_orthoTo, empty input raster" );

    kuAssert( src.size() == dst.size(), "warp_orthoTo, different count of points" );
    size_t n = src.size();
    kuAssert( n==2, "warp_orthoTo, only 2 points are supported" );

    float2 centerA = src[0];
    float2 centerB = dst[0];
    float2 a = src[1] - src[0];
    float2 b = dst[1] - dst[0];
    float lenA = a.length();
    float lenB = b.length();
    kuAssert( lenA > 0, "warp_orthoTo, bad src points - points are equal" );
    kuAssert( lenB > 0, "warp_orthoTo, bad dst points - points are equal" );
    float scale = lenB / lenA;

    a.normalize();
    b.normalize();
    float angle = a.angle( b );

    for (size_t i=0; i<n; i++) {
        kuLog << "  src " << src[i].x << ", " << src[i].y
              << ", dst " << dst[i].x << ", " << dst[i].y << endl;
    }
    kuLog << "a " << a.x << ", " << a.y << ",   b " << b.x << ", " << b.y << endl;
    kuLog << "\tscale " << scale << ", angle " << angle << endl;

    rst.allocate( W, H );
    //алгоритм ближайшего среднего //TODO реализовать и другие алгоритмы
    for ( int Y=0; Y<H; Y++ ) {
        for ( int X=0; X<W; X++ ) {
            float2 v(X,Y);  //переводим систему координат из B в A
            v -= centerB;
            v /= scale;
            v.rotate( -angle );
            v += centerA;

            int x = v.x;
            int y = v.y;
            if ( kuInAreai( x, y, w, h ) ) {
                rst.pixel( X, Y ) = pixel( x, y );
            }
        }
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::threshold( T thresh, T value0, T value1 ) {
    T* pix = pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        pix[ i ] = (pix[i] < thresh) ? value0 : value1;
    }
}

//----------------------------------------
template <typename T>
template <typename T1>
void kuRaster_<T>::copyTo( kuRaster_<T1> &rst ) {
    rst.allocate( w, h );
    T* pix = pixelsPointer();
    T1* pixOut = rst.pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        pixOut[ i ] = pix[ i ];
    }
}

//----------------------------------------
template <typename T>
template <typename T1>
void kuRaster_<T>::copyToNormalized( kuRaster_<T1> &rst, double minV, double maxV ) //нормализовать значения
{
    T min0 = minValue();
    T max0 = maxValue();

    rst.allocate( w, h );
    T* pix = pixelsPointer();
    T1* pixOut = rst.pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        pixOut[ i ] = kuMap( pix[ i ], min0, max0, minV, maxV );
    }
}

//----------------------------------------
template <typename T>
void kuRaster_<T>::normalize( double minV, double maxV, double gamma ) { //нормализовать значения

    T min0 = minValue();
    T max0 = maxValue();

    cout << "------ Normalize from " << int(min0) << " - " << int(max0) << endl;

    T* pix = pixelsPointer();
    for (size_t i=0; i<w*h; i++) {
        float v = kuMap( pix[ i ], min0, max0, 0, 1, true );
        if ( gamma > 0 && gamma != 1 ) v = pow( v, gamma ); //TODO для скорости - табулировать pow
        pix[ i ] = kuMap( v, 0, 1, minV, maxV );
    }
}
//----------------------------------------

