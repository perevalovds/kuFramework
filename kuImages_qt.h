#pragma once

//Работа с изображениями - открытие и запись в файлы png, tif, raw
#include "kuRaster.h"

const int ku_MaxTIFFSize = 32767;   //максимальный размер классического TIF-файла

//записать изображение на диск
bool kuSaveImage( kuRaster_u8 &pix, const string &fileName );

bool kuSaveImage( kuRaster_u8_3 &pix, const string &fileName );
bool kuSaveImageChannels( kuRaster_u8 &img_1, kuRaster_u8 &img_2, kuRaster_u8 &img_3, const string &fileName );


//напечатать на изображении набор строк и записать на диск изображение
void kuSaveImage_DrawText(kuRaster_u8 &pix, const string &fileName,
                           int colorRed, int colorGreen, int colorBlue,
                           string fontName, int fontSize,
                        const vector<int2> &point, const vector<string> &text );


//channel 0 - яркость, 1 - R, 2 - G, 3 - B
bool kuLoadImage( kuRaster_u8 &pix, const string &fileName, int channel = 0 );
bool kuLoadImage( kuRaster_u8_3 &pix, const string &fileName );


bool kuLoadImageFrag( kuRaster_u8 &pix, const string &fileName, kuRecti rect, int channel = 0 );


