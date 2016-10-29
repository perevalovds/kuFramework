#include "kuImages_qt.h"

#include <QColor>
#include <QImage>
#include <QPainter>

//нужно для вывода текста в изображение в kuSaveImage_DrawText
#include <QApplication>
#include <QFont>


//максимально допустимый размер изображений,
//которые мы будем созранять в TIF, BMP, PNG
//При превышении размера - сохраняем в RAW
const int IMAGE_MAX_SIZE = 32768;

//----------------------------------------
bool kuSaveImage(kuRaster_u8 &pix, const string &fileName )
{
    string ext = kuToUpper( kuFileGetExtension( fileName ) );
    if ( pix.w > 0 && pix.h > 0 ) {
        //kuLogImg( pix.w, pix.h, << "\tsaving " << fileName << endl );

        //Запрет записи размера больше ku_MaxTIFFSize для TIF и BMP
        if ( ext == "TIF" || ext == "TIFF" || ext == "BMP" ) {
            kuAssert( pix.w <= ku_MaxTIFFSize && pix.h <= ku_MaxTIFFSize,
                      "Ошибка записи файла изображения " + fileName
                      + ", размеры записываемого изображения больше максимально допустимого для данного формата файлов." );
                      //ku_tr("image_save_too_big", fileName, kuToString(pix.w), kuToString(pix.h)) );
        }

        QImage img( pix.pixelsPointer(), pix.w, pix.h, pix.w * pix.pixelSize(), QImage::Format_Indexed8 );
        //установка colormap
        QVector<QRgb> colors;
        int n = 256;
        colors.resize( n );
        for (int i=0; i<256; i++) {
            colors[i] = qRgb( i, i, i );
        }
        img.setColorTable( colors );

        img.save( fileName.c_str() );

        kuAssert( kuFileExists(fileName), "kuSaveImage - error writing file " + fileName );

        //kuLogImg( pix.w, pix.h, << "saved image" << fileName << " " << img.width() << " x " << img.height() << endl );
        //cout << "\tsaved image " << fileName << " " << img.width() << " x " << img.height() << endl;
        return true;
    }
    else {
        kuLog << "Warning: empty image for saving in '" << fileName << "'" << endl;
        return false;
    }
}


//----------------------------------------
bool kuSaveImage(kuRaster_u8_3 &pix, const string &fileName )
{
    string ext = kuToUpper( kuFileGetExtension( fileName ) );
    if ( pix.w > 0 && pix.h > 0 ) {
        //kuLogImg( pix.w, pix.h, << "\tsaving " << fileName << endl );

        //Запрет записи размера больше ku_MaxTIFFSize для TIF и BMP
        if ( ext == "TIF" || ext == "TIFF" || ext == "BMP" ) {
            kuAssert( pix.w <= ku_MaxTIFFSize && pix.h <= ku_MaxTIFFSize,
                      "Ошибка записи файла изображения " + fileName
                      + ", размеры записываемого изображения больше максимально допустимого для данного формата файлов." );
                      //ku_tr("image_save_too_big", fileName, kuToString(pix.w), kuToString(pix.h)) );
        }

        QImage img( (u8*)pix.pixelsPointer(), pix.w, pix.h, pix.w * pix.pixelSize(), QImage::Format_RGB888 );
        img.save( fileName.c_str() );

        kuAssert( kuFileExists(fileName), "kuSaveImage - error writing file " + fileName );
        return true;
    }
    else {
        kuLog << "Warning: empty image for saving in '" << fileName << "'" << endl;
        return false;
    }
}

//----------------------------------------
//напечатать на изображении набор строк и записать на диск изображение
void kuSaveImage_DrawText( kuRaster_u8 &pix, const string &fileName,
                           int colorRed, int colorGreen, int colorBlue,
                           string fontName, int fontSize,
                        const vector<int2> &point, const vector<string> &text ) {
    if ( pix.w > 0 && pix.h > 0 ) {
        kuLogImg( pix.w, pix.h, << "\tsaving image with text to " << fileName << endl );
        QImage img0( pix.pixelsPointer(), pix.w, pix.h, pix.w * pix.pixelSize(), QImage::Format_Indexed8 );
        //установка colormap
        QVector<QRgb> colors;
        int n = 256;
        colors.resize( n );
        for (int i=0; i<256; i++) {
            colors[i] = qRgb( i, i, i );
        }
        img0.setColorTable( colors );

        //конвертация в RGBA
        QImage img = img0.convertToFormat( QImage::Format_RGB32 );

        kuAssert( point.size() == text.size(), "kuSaveImage_DrawText, different sizes of points and texts" );
        if ( !point.empty() ) {
            //впечатывание текста, QApplication нужен для того, чтобы работал QFont
            int argc = 0;
            char *argv = 0;
            QApplication* app = new QApplication(argc, &argv);
            QFont font(fontName.c_str(), fontSize);     //("Arial", 30);

            QPainter painter(&img);
            painter.setPen(QColor( colorRed, colorGreen, colorBlue ));
            painter.setFont(font);

            for (size_t i=0; i<point.size(); i++) {
                painter.drawText( point[i].x, point[i].y, text[i].c_str() );
            }

            delete app;
        }
        img.save( fileName.c_str() );

        kuAssert( kuFileExists(fileName), "kuSaveImage_DrawText - error writing file " + fileName );

        //QImage img1 = img.convertToFormat( QImage::Format_RGB888 );
        //img1.save( fileName.c_str() );
        kuLogImg( pix.w, pix.h, << "saved " << fileName << " " << img.width() << " x " << img.height() << endl );
    }
    else {
        kuLog << "Warning: empty image for saving in " << fileName << endl;
    }
}



//----------------------------------------
bool kuLoadImage( kuRaster_u8 &pix, const string &fileName, int channel )
{

    kuAssert( kuFileExists( fileName ),  "kuLoadImage, file not exists " + fileName );

    QImage img;
    img.load( fileName.c_str() );
    int w = img.width();
    int h = img.height();

    kuAssert( w>0 && h>0, "File exists, but loaded as empty image, " + fileName + " (check Qt plugins)" );
    kuLogImg( w, h, << "loaded " << fileName << " " << w << " x " << h << endl );

	pix.allocate( w, h, 0 );

    int step = 10000000;
    int cnt = 0;

    kuLogImg( w, h, << "converting to raster..." << endl );
    int col = 0;
	for (int i=0; i<w*h; i++) {
        QRgb color = img.pixel( i % w, i / w );
        if ( channel == 0 ) col = QColor::fromRgb( color ).lightness();
        if ( channel == 1 ) col = qRed( color );    //QColor::fromRgb( color ).red();
        if ( channel == 2 ) col = qGreen( color );  //QColor::fromRgb( color ).green();
        if ( channel == 3 ) col = qBlue( color );   //QColor::fromRgb( color ).blue();
        pix.pixelsPointer()[i] = col;

        if ( w*h > kuLog_ImageSize ) {
            if ( cnt==0 ) {
                cnt = step;
                kuLog << int( 100.0 * i / (w*h) ) << " %  \t" << endl << flush;
            }
            cnt--;
        }
    }
    kuLogImg( w, h, << endl << "converted " << fileName << " " << w << " x " << h << endl );

    return true;
}


//----------------------------------------
bool kuLoadImage( kuRaster_u8_3 &pix, const string &fileName ) {
    kuAssert( kuFileExists( fileName ),  "kuLoadImage, file not exists " + fileName );

    QImage img;
    img.load( fileName.c_str() );
    int w = img.width();
    int h = img.height();

    kuAssert( w>0 && h>0, "File exists, but loaded as empty image, " + fileName + " (check Qt plugins)" );
    kuLogImg( w, h, << "loaded " << fileName << " " << w << " x " << h << endl );

    pix.allocate( w, h, 0 );

    int step = 10000000;
    int cnt = 0;

    kuLogImg( w, h, << "converting to raster..." << endl );
    for (int i=0; i<w*h; i++) {
        QRgb color = img.pixel( i % w, i / w );
        pix.pixelsPointer()[i] = u8_3(qRed(color), qGreen(color), qBlue(color));

        if ( w*h > kuLog_ImageSize ) {
            if ( cnt==0 ) {
                cnt = step;
                kuLog << int( 100.0 * i / (w*h) ) << " %  \t" << endl << flush;
            }
            cnt--;
        }
    }
    kuLogImg( w, h, << endl << "converted " << fileName << " " << w << " x " << h << endl );

    return true;

}

//----------------------------------------
bool kuLoadImageFrag( kuRaster_u8 &pix, const string &fileName, kuRecti rect, int channel )
{
    kuAssert( kuFileExists( fileName ),  "kuLoadImageFrag: file not exists, " + fileName );

    QImage img;
    img.load( fileName.c_str() );
    int W = img.width();
    int H = img.height();

    kuLog << "loaded " << fileName << " " << W << " x " << H << endl;

    kuAssert( W > 0 && H > 0, "kuLoadImageFrag: file exists, but loaded as empty image, " + fileName );

    int w = rect.w;
    int h = rect.h;
    pix.allocate( w, h, 0 );

    int step = 10000000;
    int cnt = 0;

    kuLog << "converting to raster (fragment " << rect.x << "," << rect.y << " " << rect.w << "x" << rect.h << ")..." << endl;
    int col = 0;
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            int X = x + rect.x;
            int Y = y + rect.y;
            if ( kuInAreai( X, Y, W, H ) ) {
                QRgb color = img.pixel( X, Y );
                if ( channel == 0 ) col = QColor::fromRgb( color ).lightness();
                if ( channel == 1 ) col = qRed( color );    //QColor::fromRgb( color ).red();
                if ( channel == 2 ) col = qGreen( color );  //QColor::fromRgb( color ).green();
                if ( channel == 3 ) col = qBlue( color );   //QColor::fromRgb( color ).blue();
                pix.pixelsPointer()[ x + w * y ] = col;
            }
            if ( cnt==0 ) {
                cnt = step;
                kuLog << int( 100.0 * (x+w*y) / (w*h) ) << " %  \t" << endl << flush;
            }
            cnt--;
        }
    }
    kuLog << endl;
    kuLog << "converted " << fileName << " " << w << " x " << h << endl;

    return true;

}

//----------------------------------------
bool kuSaveImageChannels( kuRaster_u8 &img_1, kuRaster_u8 &img_2, kuRaster_u8 &img_3, const string &fileName )
{
   if ( img_1.w > 0 && img_1.h > 0 && img_2.w > 0 && img_2.h > 0 && img_3.w > 0 && img_3.h > 0 ) {
        kuLog << "\tsaving_color " << fileName << endl;

        int W = img_1.w, H = img_1.h;
        vector<unsigned char> img( W * H * 3, 0 );
        for ( int i = 0; i < 3*W*H; i += 3 ){
            img[ i ]     = img_1.pixel( (i/3)%W, (i/3)/W );
            img[ i + 1 ] = img_2.pixel( (i/3)%W, (i/3)/W );
            img[ i + 2 ] = img_3.pixel( (i/3)%W, (i/3)/W );
        }

        QImage picture( &img[0], W, H, 3 * W, QImage::Format_RGB888 );
        picture.save( QString::fromUtf8( fileName.data(), fileName.size() ) );

        img.clear();

        kuAssert( kuFileExists(fileName), "kuSaveImageColor - error writing file " + fileName );

        kuLog << "saved " << fileName << " " << picture.width() << " x " << picture.height() << endl;
        return true;
    }
    else {
        kuLog << "Warning: empty image for saving in " << fileName << endl;
        return false;
    }
}


//----------------------------------------
