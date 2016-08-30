#include "kuRaster.h"
#include "kuMain.h"

//Обработка растров

//-----------------------------------------------------------------
//билинейная интерполяция, возвращает float-биты
void kuGetBilinearBits_float8( kuRaster_u8 &img, float x, float y, float8 &out ) {
    int x0 = kuClampi( int(x), 0, img.w-1 );
    int y0 = kuClampi( int(y), 0, img.h-1 );
    int x1 = kuClampi( x0+1, 0, img.w-1 );
    int y1 = kuClampi( y0+1, 0, img.h-1 );
    x = kuClamp( x-x0, 0, 1 );
    y = kuClamp( y-y0, 0, 1 );
    int w = img.w;
    //int h = img.h;

    u8 *pix = img.pixelsPointer();
    int bit = 1;
    for (int i=0; i<8; i++) {
        out.v[i] = BIT_TO_INT( pix[x0+w*y0], bit ) * (1-x)*(1-y)
                + BIT_TO_INT( pix[x1+w*y0], bit ) * (x)*(1-y)
                + BIT_TO_INT( pix[x0+w*y1], bit ) * (1-x)*(y)
                + BIT_TO_INT( pix[x1+w*y1], bit ) * (x)*(y);
        bit += bit;
    }
}


//-----------------------------------------------------------------
//билинейная интерполяция, возвращает обычные биты - пороговая обработка
u8 kuGetBilinearBits_u8( kuRaster_u8 &img, float x, float y, float thresh ) {
    float8 floatBits;
    kuGetBilinearBits_float8( img, x, y, floatBits );

    u8 out = 0;
    int bit = 1;
    for (int i=0; i<8; i++) {
        if ( floatBits.v[i] > thresh ) {
            out += bit;
        }
        bit += bit;
    }
    return out;
}

//-----------------------------------------------------------------
//Умножение на скаляр
void kuRasterMult( kuRaster_f32 &img, float koef ) {
    for (int i=0; i<img.w * img.h; i++) {
        img.pixelsPointer()[i] *= koef;
    }
}

//-----------------------------------------------------------------
void kuRasterMult( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &res ) { //Попиксельное умножение
	kuAssert( a.equalSize(b), "kuRasterMult - different image sizes" );
	res.allocate(a.w, a.h);
    for (int i=0; i<a.w * a.h; i++) {
        res.pixels()[i] = a.pixels()[i] * b.pixels()[i];
    }
}

//-----------------------------------------------------------------
void kuRasterAdd( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &res, float ka, float kb ) { //Попиксельное суммирование с весами
	kuAssert( a.equalSize(b), "kuRasterMult - different image sizes" );
	res.allocate(a.w, a.h);
    for (int i=0; i<a.w * a.h; i++) {
        res.pixels()[i] = ka * a.pixels()[i] + kb * b.pixels()[i];
    }
}

//-----------------------------------------------------------------
void kuRasterAdd( kuRaster_f32 &img, float add ) {
    for (int i=0; i<img.w * img.h; i++) {
        img.pixelsPointer()[i] += add;
    }
}

//-----------------------------------------------------------------
void kuRasterMin( kuRaster_f32 &a, kuRaster_f32 &b, kuRaster_f32 &out ) {
    if ( !out.equalSize( a ) ) {
        out.allocate( a.w, a.h );
    }
    for (int i=0; i<a.w * a.h; i++) {
        out.pixelsPointer()[i] = min( a.pixelsPointer()[i], b.pixelsPointer()[i] );
    }
}

//-----------------------------------------------------------------
//Медианная фильтрация

/*float kuMedianValue( float *v, int n ) {
    //метод взятия среднего - с ним работает хуже
    if  (n > 0 ) {
        if ( n % 2 == 1 ) return v[n/2];
        else return (v[n/2] + v[n/2+1]) * 0.5;
    }
    else return 0;
}*/

//-----------------------------------------------------------------
void kuRasterMedian( kuRaster_u8 &img, kuRaster_u8 &imgOut, int winRad ) {
    kuLog << "Median filtration..." << endl;
    kuAssert( winRad >= 0, "kuRasterMedian winRad " + kuToString( winRad ) );
    int w = img.w;
    int h = img.h;
    u8 *pix = img.pixelsPointer();

    imgOut.allocate( w, h, 0 );
    u8 *pixout = imgOut.pixelsPointer();

    //TODO!! граничные точки обработать, а то сейчас они чёрными выдаются
    int winSize = 2*winRad + 1;
    int N = winSize * winSize;
    vector<u8> valuesV( N );
    u8 *values = &valuesV[0];

    for (int y=0; y<h-winSize; y++) {
        if ( y % 100 == 0 ) { kuLog << "\t" << y << " / " << h << endl; }
        for (int x=0; x<w-winSize; x++) {
            int cnt = 0;
            for (int b=0; b<winSize; b++) {
                for (int a=0; a<winSize; a++) {
                    values[ cnt++ ] = pix[ x+a + w*(y+b) ];
                }
            }
            qsort( values, cnt, sizeof(values[0]), kuCompareU8_pointers );
            pixout[ x + winRad + w * (y + winRad) ] = values[ cnt / 2 ];    //kuMedianValue( values, cnt );
        }
    }
}

//-----------------------------------------------------------------
void kuRasterMedian( kuRaster_f32 &img, kuRaster_f32 &imgOut, int winRad ) {
    kuLog << "Median filtration..." << endl;
    kuAssert( winRad >= 0, "kuRasterMedian winRad " + kuToString( winRad ) );
    int w = img.w;
    int h = img.h;
    f32 *pix = img.pixelsPointer();

    imgOut.allocate( w, h, 0 );
    f32 *pixout = imgOut.pixelsPointer();

    //TODO!! граничные точки обработать, а то сейчас они чёрными выдаются
    int winSize = 2*winRad + 1;
    int N = winSize * winSize;
    vector<f32> valuesV( N );
    float *values = &valuesV[0];

    for (int y=0; y<h-winSize; y++) {
        if ( y % 100 == 0 ) { kuLog << "\t" << y << " / " << h << endl; }
        for (int x=0; x<w-winSize; x++) {
            int cnt = 0;
            for (int b=0; b<winSize; b++) {
                for (int a=0; a<winSize; a++) {
                    values[ cnt++ ] = pix[ x+a + w*(y+b) ];
                }
            }
            qsort( values, cnt, sizeof(values[0]), kuCompareFloat_pointers );
            pixout[ x + winRad + w * (y + winRad) ] = values[ cnt / 2 ];    //kuMedianValue( values, cnt );
        }
    }
}

//-----------------------------------------------------------------
void kuRastersMedian( kuRaster_f32 &imgA, kuRaster_f32 &imgB, kuRaster_f32 &imgOut, int winRad, float BadValue ) //Медианная фильтрация двух растров
{
    kuLog << "Median filtration, two rasters..." << endl;
    kuAssert( winRad >= 0, "kuRasterMedian winRad " + kuToString( winRad ) );
    int w = imgA.w;
    int h = imgA.h;
    f32 *pixA = imgA.pixelsPointer();
    f32 *pixB = imgB.pixelsPointer();

    imgOut.allocate( w, h, 0 );
    f32 *pixout = imgOut.pixelsPointer();

    //TODO!! граничные точки обработать
    int winSize = 2*winRad + 1;
    int N = winSize * winSize * 2;
    vector<f32> valuesV( N );
    float *values = &valuesV[0];

    for (int y=0; y<h-winSize; y++) {
        if ( y % 100 == 0 ) { kuLog << "\t" << y << " / " << h << endl; }

        for (int x=0; x<w-winSize; x++) {
            int cnt = 0;
            for (int b=0; b<winSize; b++) {
                for (int a=0; a<winSize; a++) {
                    float &valueA = pixA[ x+a + w*(y+b) ];
                    float &valueB = pixB[ x+a + w*(y+b) ];
                    if ( int(valueA) != int(BadValue) ) values[ cnt++ ] = valueA;
                    if ( int(valueB) != int(BadValue) ) values[ cnt++ ] = valueB;
                }
            }
            if ( cnt > 0 ) {
                qsort( values, cnt, sizeof(values[0]), kuCompareFloat_pointers );
                pixout[ x + winRad + w * (y + winRad) ] = values[ cnt / 2 ]; //kuMedianValue( values, cnt );
            }
            else {
                pixout[ x + winRad + w * (y + winRad) ] = BadValue;
            }
        }
    }

}

//-----------------------------------------------------------------
//Медианная фильтрация, результат квантиль, то есть 0-min, 0.5-медиана, 1.0-max
//Работает медленно, поэтому можно сжимать - shrink.
void kuRasterMedianQuantile( kuRaster_f32 &rst, kuRaster_f32 &rstOut,
                        int winRad, float quantile, int shrink, float IgnoreValue ) {
    kuAssert( winRad >= 0, "kuRasterMedianQuantile, bad winRad" );
    kuAssert( kuInRange( quantile, 0, 1 ), "kuRasterMedianQuantile, bad quantile" );
    kuAssert( shrink >= 1, "kuRasterMedianQuantile, bad shrink" );

    int w = rst.w;
    int h = rst.h;
    f32 *pix = rst.pixelsPointer();

    int W = w / shrink;
    int H = h / shrink;
    kuAssert( W > 0 && H > 0, "kuRasterMedianQuantile, input image too small for shrinking" );

    rstOut.allocate( W, H, 0 );
    f32 *pixout = rstOut.pixelsPointer();

    int winSize = 2*winRad + 1;
    int N = winSize * winSize;
    vector<f32> valuesV( N );
    float *values = &valuesV[0];

    //TODO круглую окрестность, с наложением границ и разными весами (гауссиан)
    //Получится робастный метод поиска минимума, гладкого рельефа
    for (int Y=0; Y<H; Y++) {
        if ( Y % 4 == 0 ) {
            kuLog << " " << Y * 100 / H << "%" << endl;
        }
        for (int X=0; X<W; X++) {
            int x = X * w / W;
            int y = Y * h / H;
            int x0 = kuClampi( x - winRad, 0, w-1 );
            int x1 = kuClampi( x + winRad, 0, w-1 );
            int y0 = kuClampi( y - winRad, 0, h-1 );
            int y1 = kuClampi( y + winRad, 0, h-1 );
            int cnt = 0;
            for (int b=y0; b<=y1; b++) {
                for (int a=x0; a<=x1; a++) {
                    if ( pix[ a + w*b ] != IgnoreValue ) {
                        values[ cnt++ ] = pix[ a + w*b ];
                    }
                }
            }
            if ( cnt > 0 ) {
                qsort( values, cnt, sizeof(values[0]), kuCompareFloat_pointers );

                int index = kuClamp( quantile * cnt, 0, cnt-1 );
                pixout[ X + W * Y ] = values[ index ];
            }
            else {
                pixout[ X + W * Y ] = IgnoreValue;
            }
        }
    }
    kuLog << endl;
}

//-----------------------------------------------------------------
//Сглаживание, только в точках маски
//Предполагается, что используется для сглаживания скачков между большими плоскими поверхностями
//shrink - сжимает внутреннюю картинку для ускорения, хорошо для гладких поверхностей
void kuRasterSmoothInMask( kuRaster_f32 &in, kuRaster_f32 &out, kuRaster_u8 &mask,
                        int winRad, int shrink, float dopusk ) {
    kuAssert( winRad >= 0, "kuRasterMedianInMask, bad winRad" );
    kuAssert( shrink >= 1, "kuRasterMedianInMask, bad shrink" );

    int w = in.w;
    int h = in.h;
    f32 *in_ = in.pixelsPointer();
    u8 *mask_ = mask.pixelsPointer();


    int W = w / shrink;
    int H = h / shrink;
    kuAssert( W > 0 && H > 0, "kuRasterMedianInMask, input image too small for shrinking" );

    kuRaster_f32 shrinked;

    shrinked.allocate( W, H, 0 );

    //int winSize = 2*winRad + 1;
    //int N = winSize * winSize;
    //vector<f32> valuesV( N );
    //float *values = &valuesV[0];

    kuGauss1D gauss;
    float sigma = winRad*0.5;   //TODO параметр
    gauss.setup( winRad, sigma );
    vector<double> filter_ = gauss.filter();
    double *filter = &filter_[0];


    //строим грубую, но гладкую поверхность, используя медианную фильтрацию с маской
    for (int Y=0; Y<H; Y++) {
        if ( Y % 10 == 0 ) { kuLog << "\t" << Y << " / " << H << endl; }
        for (int X=0; X<W; X++) {
            int x = X * w / W;
            int y = Y * h / H;

            //float Value0 = ( mask_[x+w*y] ) ? in_[x+w*y] : 0;   //TODO определяем "ключевую" высоту, тут надо провести поиск в окрестности,
            //если в самой точке маска равно 0


            int x0 = kuClampi( x - winRad, 0, w-1 );
            int x1 = kuClampi( x + winRad, 0, w-1 );
            int y0 = kuClampi( y - winRad, 0, h-1 );
            int y1 = kuClampi( y + winRad, 0, h-1 );
            double cnt = 0;
            double sum = 0;
            int xBase = x - winRad;
            int yBase = y - winRad;
            for (int b=y0; b<=y1; b++) {
                for (int a=x0; a<=x1; a++) {
                    float &v = in_[ a+w*b ];
                    if ( mask_[ a + w*b ] /*&& fabs( v - Value0 ) <= dopusk*/ ) {
                        double wgh = filter[a-xBase] * filter[b-yBase];
                        sum += wgh * v;
                        cnt += wgh;
                        //values[ cnt++ ] = in_[ a + w*b ];
                    }
                }
            }
            if ( cnt > 0 ) {            //TODO если cnt==0, то пересчитать с большим радиусом
                //qsort( values, cnt, sizeof(values[0]), kuCompareFloat_pointers );
                //shrinked.pixel( X, Y ) = values[ cnt/2 ];
                shrinked.pixel( X, Y ) = sum / cnt;
            }
        }
    }

    //вносим точки построенной поверхности в результат, в точках маски
    out.allocate( w, h );
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            float res =  in.pixel( x, y );
            if ( mask_[x+w*y] ) {
                float v = shrinked.pixelInterp( float(x) * W / w, float(y) * H / h, KU_INTERP_BICUBIC );
                //if ( fabs( v - in.pixel(x,y)) <= dopusk ) {
                res = v;

            }
            out.pixel( x, y ) = res;
        }
    }

}

//-----------------------------------------------------------------
//можно выполнять in-place
void kuRasterAnd( kuRaster_u8 &img1, kuRaster_u8 &img2, kuRaster_u8 &imgOut ) {
    kuAssert( img1.equalSize( img2 ), "kuRasterAnd - different size of images" );
    int w = img1.w;
    int h = img1.h;
    if ( !imgOut.equalSize( img1 ) ) {
        imgOut.allocate( w, h );
    }
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            imgOut.pixel( x, y ) = img1.pixel(x,y) & img2.pixel(x,y);
        }
    }
}

//-----------------------------------------------------------------
int kuRasterCountNonZero( kuRaster_u8 &img, int X, int Y, int w, int h ) {
    kuAssert( w >= 0 && h >= 0 && X >= 0 && X+w <= img.w
              && Y >= 0 && Y+h <= img.h, "kuRasterCountNonZero, bad region" );
    int count = 0;
    for (int y=Y; y<Y+h; y++) {
        for (int x=X; x<X+w; x++) {
            if ( img.pixel(x,y) != 0 ) { count++; }
        }
    }
    return count;
}

//-----------------------------------------------------------------
kuRecti kuRasterRectNonZero( kuRaster_u8 &img, int X, int Y, int w, int h ) {
    kuAssert( w >= 0 && h >= 0 && X >= 0 && X+w <= img.w
              && Y >= 0 && Y+h <= img.h, "kuRasterRectNonZero, bad region" );
    int x0 = X + w;
    int y0 = Y + h;
    int x1 = X-1;
    int y1 = Y-1;
    for (int y=Y; y<Y+h; y++) {
        for (int x=X; x<X+w; x++) {
            if ( img.pixel(x,y) != 0 ) {
                x0 = min( x0, x );
                x1 = max( x1, x );
                y0 = min( y0, y );
                y1 = max( y1, y );
            }
        }
    }
    return kuRecti( x0, y0, x1 - x0 + 1, y1 - y0 + 1 );
}

//-----------------------------------------------------------------
//поиск связной sv-компоненты цвета == search, перекрашивает в fillColor
size_t kuFloodFill( kuRaster_u8 &input, int sv,
                 int x0, int y0, int search, int fillColor, vector<int2> *outPoints ) {
    int w = input.w;
    int h = input.h;

    vector<int2> res;
    res.push_back( int2(x0, y0) );
    input.pixel( x0, y0 ) = fillColor;

    unsigned int begin = 0;
    while (begin < res.size()) {
        int2 &p = res[begin];
        int px = p.x;
        int py = p.y;

        begin++;
        for (int y = py - 1; y <= py + 1; y++)  {
            for (int x = px - 1; x <= px + 1; x++) {
                if (x >= 0 && x < w && y >= 0 && y < h
                        && (sv==8 || x == px || y == py)
                        && input.pixel( x, y ) == search ) {
                    res.push_back( int2(x,y) );
                    input.pixel( x, y ) = fillColor;
                }

            }
        }
    }

    if ( outPoints ) { *outPoints = res; }
    return res.size();
}

//-----------------------------------------------------------------
size_t kuFloodFill( kuRaster_f32 &input, int sv,
                 int x0, int y0, float search, kuRaster_u8 &mask, int fillColor, vector<int2> *outPoints ) {
    if ( mask.pixel(x0, y0) == fillColor ) { return 0; }

    int w = input.w;
    int h = input.h;

    vector<int2> res;
    res.push_back( int2(x0, y0) );
    mask.pixel( x0, y0 ) = fillColor;

    unsigned int begin = 0;
    while (begin < res.size()) {
        int2 &p = res[begin];
        int px = p.x;
        int py = p.y;

//        if ( outPoints ) { outPoints->push_back( p ); }

        begin++;
        for (int y = py - 1; y <= py + 1; y++)  {
            for (int x = px - 1; x <= px + 1; x++) {
                if (x >= 0 && x < w && y >= 0 && y < h
                        && (sv==8 || x == px || y == py)
                        && input.pixel( x, y ) == search && mask.pixel( x, y ) != fillColor ) {
                    res.push_back( int2(x,y) );
                    mask.pixel( x, y ) = fillColor;
                }

            }
        }
    }

    if ( outPoints ) { *outPoints = res; }
    return res.size();
}


//-----------------------------------------------------------------
//template <typename T, typename T2>
size_t kuFloodFill_( kuRaster_u8 &input, kuRaster_f32 &input2, int sv,
                 int x0, int y0, u8 search, u8 fillColor, f32 search2, vector<int2> *outPoints ) {
    //if ( outPoints ) { outPoints->clear(); }

    int w = input.w;
    int h = input.h;

    vector<int2> res;
    res.push_back( int2(x0, y0) );
    input.pixel( x0, y0 ) = fillColor;

    unsigned int begin = 0;
    while (begin < res.size()) {
        int2 &p = res[begin];
        int px = p.x;
        int py = p.y;

        begin++;
        for (int y = py - 1; y <= py + 1; y++)  {
            for (int x = px - 1; x <= px + 1; x++) {
                if (x >= 0 && x < w && y >= 0 && y < h
                        && (sv==8 || x == px || y == py)
                        && input.pixel( x, y ) == search
                        && input2.pixel( x, y ) == search2
                        ) {
                    res.push_back( int2(x,y) );
                    input.pixel( x, y ) = fillColor;
                }

            }
        }
    }

    if ( outPoints ) { *outPoints = res; }
    return res.size();
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Фильтрация
kuGauss1D::kuGauss1D() {
    _inited = false;
}

//-----------------------------------------------------------------
void kuGauss1D::setup( int rad, double sigma )
{
    kuAssert( rad > 0, "kuGauss1D, bad rad " + kuToString( rad ) );
    kuAssert( sigma > 0, "kuGauss1D, bad sigma " + kuToString( sigma ) );
    _inited = true;
    _rad = rad;
    _sigma = sigma;
    _n = 2 * _rad + 1;
    _filter.resize( _n );
    double sum = 0;
    for (int i=0; i<_n; i++) {
        double v = i - _rad;
        v = exp( - kuSqr(v) / (2 * kuSqr(_sigma) ) );
        _filter[i] = v;
        sum += v;
    }
    //нормализация
    if ( sum > 0 ) {
        for (int i=0; i<_n; i++) {
            _filter[i] /= sum;
        }
    }
 }

//-----------------------------------------------------------------
string kuGauss1D::print( bool full ) {
    string s = "kuGauss1D rad=" + kuToString( _rad ) + ", sigma=" + kuToString( _sigma );
    if ( full ) {
        for (int i=0; i<_n; i++) {
            s += "\t" + kuToString( _filter[i] );
        }
    }
    return s;
}

//-----------------------------------------------------------------
