#pragma once

//Операции с растрами
#include "kuRaster.h"


//билинейная интерполяция, возвращает float-биты
void kuGetBilinearBits_float8( kuRaster_u8 &img, float x, float y, float8 &out );
//билинейная интерполяция, возвращает обычные биты - пороговая обработка
u8 kuGetBilinearBits_u8( kuRaster_u8 &img, float x, float y, float thresh = 0.5 );



void kuRasterMult( kuRaster_f32 &img, float koef ); //Умножение на скаляр
void kuRasterMult( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &res ); //Попиксельное умножение

void kuRasterAdd( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &res, float ka, float kb );
void kuRasterAdd( kuRaster_f32 &img, float add );
void kuRasterMin( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &out );

void kuRasterMedian( kuRaster_u8 &img, kuRaster_u8 &imgOut, int winRad );
void kuRasterMedian( kuRaster_f32 &img, kuRaster_f32 &imgOut, int winRad ); //Медианная фильтрация
void kuRastersMedian( kuRaster_f32 &imgA, kuRaster_f32 &imgB, kuRaster_f32 &imgOut, int winRad, float BadValue ); //Медианная фильтрация двух растров


//Медианная фильтрация, результат квантиль, то есть 0-min, 0.5-медиана, 1.0-max
//Работает медленно, поэтому можно сжимать - shrink.
void kuRasterMedianQuantile(kuRaster_f32 &rst, kuRaster_f32 &rstOut,
                        int winRad, float quantile, int shrink , float IgnoreValue);


//Сглаживание, только в точках маски
//Предполагается, что используется для сглаживания скачков между большими плоскими поверхностями
//shrink - сжимает внутреннюю картинку для ускорения, хорошо для гладких поверхностей
void kuRasterSmoothInMask(kuRaster_f32 &in, kuRaster_f32 &out, kuRaster_u8 &mask,
                        int winRad, int shrink, float dopusk );

//можно выполнять in-place
void kuRasterAnd( kuRaster_u8 &img1, kuRaster_u8 &img2, kuRaster_u8 &imgOut );
int kuRasterCountNonZero( kuRaster_u8 &img, int x, int y, int w, int h );
kuRecti kuRasterRectNonZero( kuRaster_u8 &img, int x, int y, int w, int h );

//поиск связной sv-компоненты цвета == search, перекрашивает в fillColor
size_t kuFloodFill(kuRaster_u8 &input, int sv,
                 int x0, int y0, int search, int fillColor, vector<int2> *outPoints = 0 );

size_t kuFloodFill( kuRaster_f32 &input, int sv,
                 int x0, int y0, float search, kuRaster_u8 &mask, int fillColor, vector<int2> *outPoints = 0 );

size_t kuFloodFill_( kuRaster_u8 &input, kuRaster_f32 &input2, int sv,
                 int x0, int y0, u8 search, u8 fillColor, f32 search2, vector<int2> *outPoints );

//удаление шума на маске
void kuNoiseRemove( kuRaster_u8 &input, kuRaster_u8 &output, int noise_remove );

//Работа с цветными изображениями
void kuRasterDesaturate( kuRaster_u8_3 &input, kuRaster_u8 &gray );

void kuRasterSplit( kuRaster_u8_3 &input, vector<kuRaster_u8> &rgb );
void kuRasterMerge( vector<kuRaster_u8> &rgb, kuRaster_u8_3 &output );

//Изменение размеров цветного изображения
void kuRasterResize( kuRaster_u8_3 &input, kuRaster_u8_3 &output, int w, int h, int interp_type );


//Фильтрация
struct kuGauss1D {
    kuGauss1D();
    void setup( int rad, double sigma );
    template <typename T>
    double get( T *data, int w, int center );
    const vector<double> &filter() { return _filter; }
    string print( bool full = false );
private:
    bool _inited;
    int _rad, _n;
    double _sigma;
    vector<double> _filter;
};

//----------------------------------------
template <typename T>
double kuGauss1D::get( T *data, int w, int center ) {
    kuAssert( _inited, "kuGauss1D::get - is not inited" );
    int x = center - _rad;
    kuAssert( x >= 0 && x + _n <= w, "kuGauss1D::get out of bounds, center=" + kuToString( center ) );
    double sum = 0;
    double *filter = &_filter[0];
    for (int i=0; i<_n; i++) {
        sum += filter[i] * data[ x + i ];
    }
    return sum;
}


//Гауссово сглаживание изображений
template <typename T>
void kuFilterGauss( kuRaster_<T> &in, kuRaster_<T> &out, int2 rad, float2 sigma ) {
	kuGauss1D gaussX, gaussY;
	gaussX.setup( rad.x, sigma.x );
	gaussY.setup( rad.y, sigma.y );
	vector<double> filterX = gaussX.filter();
	vector<double> filterY = gaussY.filter();
	int w = in.w;
	int h = in.h;
	kuRaster_f32 temp;
	temp.allocate(w,h);

	//TODO сделать расширение изображения, чтобы до края сглаживал
	//сглаживание по x
	for (int y=0; y<h; y++) {
		for (int x=0; x<w-filterX.size(); x++) {
			double s = 0;
			for (int q=0; q<filterX.size(); q++) {
				s += in.pixel(x+q,y) * filterX[q];
			}
			temp.pixel(x+rad.x,y) = s;
		}
	}
	//сглаживание по y
	out.allocate(w,h);
	for (int x=0; x<w; x++) {
		for (int y=0; y<h-filterY.size(); y++) {
			double s = 0;
			for (int q=0; q<filterY.size(); q++) {
				s += temp.pixel(x,y+q) * filterY[q];
			}
			out.pixel(x,y+rad.y) = s;
		}
	}
}

//----------------------------------------


