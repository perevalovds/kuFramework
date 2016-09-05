#include "kuImages_of.h"


//------------------------------------------------------------------------
bool kuSaveImage( kuRaster_u8 &pix, const string &fileName ) {
	ofPixels px;
	kuSaveImage( pix, px );
	ofSaveImage( px, fileName );
	return true;
}

//------------------------------------------------------------------------
bool kuSaveImage( kuRaster_u8_3 &pix, const string &fileName ) {
	ofPixels px;
	kuSaveImage( pix, px );
	ofSaveImage( px, fileName );	
	return true;
}

//------------------------------------------------------------------------
bool kuLoadImage( kuRaster_u8 &pix, const string &fileName ) {
	ofPixels px;
	ofLoadImage( px, fileName );
	px.setImageType( OF_IMAGE_GRAYSCALE );
	kuLoadImage( px, pix );
	return true;
}

//------------------------------------------------------------------------
bool kuLoadImage( kuRaster_u8_3 &pix, const string &fileName ) {
	ofPixels px;
	ofLoadImage( px, fileName );
	kuLoadImage( px, pix );
	return true;
}

//------------------------------------------------------------------------
void kuSaveImage( kuRaster_u8 &img, ofPixels &pixels ) {
	int w = img.w;
	int h = img.h;
	pixels.allocate( w, h, OF_IMAGE_GRAYSCALE );
	for (int i=0; i<w*h; i++) {
		pixels.getPixels()[i] = img.pixels()[i];
	}
}

//------------------------------------------------------------------------
void kuLoadImage( ofPixels &pixels, kuRaster_u8 &img ) {
	img.clear();
	if ( pixels.getNumChannels() == 1 ) {
		int w = pixels.getWidth();
		int h = pixels.getHeight();
		img.allocate( w, h );
		for (int i=0; i<w*h; i++) {
			img.pixels()[i] = pixels.getPixels()[i];
		}
	}
}

//------------------------------------------------------------------------
void kuSaveImage( kuRaster_u8_3 &img, ofPixels &pixels ) {
	int w = img.w;
	int h = img.h;
	pixels.allocate( w, h, OF_IMAGE_COLOR );
	for (int i=0; i<w*h; i++) {
		pixels.getPixels()[3*i+0] = img.pixels()[i].v.v[0];
		pixels.getPixels()[3*i+1] = img.pixels()[i].v.v[1];
		pixels.getPixels()[3*i+2] = img.pixels()[i].v.v[2];
	}
}

//------------------------------------------------------------------------
void kuLoadImage( ofPixels &pixels, kuRaster_u8_3 &img ) {
	img.clear();
	if ( pixels.getNumChannels() == 3 ) {
		int w = pixels.getWidth();
		int h = pixels.getHeight();
		img.allocate( w, h );
		for (int i=0; i<w*h; i++) {
			img.pixels()[i].v.v[0] = pixels.getPixels()[3*i+0];
			img.pixels()[i].v.v[1] = pixels.getPixels()[3*i+1];
			img.pixels()[i].v.v[2] = pixels.getPixels()[3*i+2];
		}
	}
}

//------------------------------------------------------------------------
