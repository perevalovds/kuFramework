#pragma once

//Работа с изображениями, для openFrameworks
#include "ofMain.h"
#include "kuRaster.h"

//записать изображение на диск
bool kuSaveImage( kuRaster_u8 &pix, const string &fileName );
bool kuSaveImage( kuRaster_u8_3 &pix, const string &fileName );

//считать изображение с диска
bool kuLoadImage( kuRaster_u8 &pix, const string &fileName );
bool kuLoadImage( kuRaster_u8_3 &pix, const string &fileName );

void kuSaveImage( kuRaster_u8 &img, ofPixels &pixels );
void kuLoadImage( ofPixels &pixels, kuRaster_u8 &img );

void kuSaveImage( kuRaster_u8_3 &img, ofPixels &pixels );
void kuLoadImage( ofPixels &pixels, kuRaster_u8_3 &img );
