#include "kuRaster.h"
#include "kuMain.h"

//----------------------------------------
kuRasterHeader::kuRasterHeader() {
    setVersion("");
    setType( kuRaster_type_undefined );
    w = h = pixelSize = 0;
}

//----------------------------------------
kuRasterHeader::kuRasterHeader( const string &version0, int type0,
                                int pixelSize0, int w0, int h0 ) {
    setVersion( version0 );
    setType( type0 );
    w = w0;
    h = h0;
    pixelSize = pixelSize0;
}

//----------------------------------------
void kuRasterHeader::saveToFILE( FILE *f ) {
    fwrite( this, sizeof( kuRasterHeader ), 1, f );
    kuAssert( !ferror(f), "kuRasterHeader::saveToFILE - error writing to file" );
}

//----------------------------------------
void kuRasterHeader::loadFromFILE( FILE *f ) {
    fread( this, sizeof( kuRasterHeader ), 1, f );
    kuAssert( !ferror(f), "kuRasterHeader::loadFromFILE - error loading from file" );

}

//----------------------------------------
void kuRasterHeader::loadFromRstFile( const string &fileName ) {
    FILE *f = fopen( fileName.c_str(), "rb ");
    kuAssert( f!=0, "kuRasterHeader::loadFromRstFile - error loading from " + fileName );
    loadFromFILE( f );
    fclose( f );
}

//----------------------------------------
string kuRasterHeader::version() {
    return kuBufferToString( _version, kuRasterHeader_versionLen );
}

//----------------------------------------
int kuRasterHeader::type() {
    string s = kuBufferToString( _type, kuRasterHeader_typeLen );
    int t = kuRaster_type_undefined;
    if ( s == "u8" ) t = kuRaster_type_u8;
    if ( s == "s16" ) t = kuRaster_type_s16;
    if ( s == "s32" ) t = kuRaster_type_s32;
    if ( s == "f32" ) t = kuRaster_type_f32;
    if ( s == "u16" ) t = kuRaster_type_u16;
    return t;
}

//----------------------------------------
void kuRasterHeader::setVersion( string v ) {
    kuStringToBuffer( v, _version, kuRasterHeader_versionLen );
}

//----------------------------------------
void kuRasterHeader::setType( int t ) {
    string s = "";
    if ( t == kuRaster_type_u8 ) s = "u8";
    if ( t == kuRaster_type_s16 ) s = "s16";
    if ( t == kuRaster_type_s32 ) s = "s32";
    if ( t == kuRaster_type_f32 ) s = "f32";
    if ( t == kuRaster_type_u16 ) s = "u16";
    kuStringToBuffer( s, _type, kuRasterHeader_typeLen );
}


//----------------------------------------
//----------------------------------------
//----------------------------------------
template <>
int kuRaster_<u8>::type() {
    return kuRaster_type_u8;
}
//----------------------------------------
template <>
int kuRaster_<s16>::type() {
    return kuRaster_type_s16;
}
//----------------------------------------
template <>
int kuRaster_<s32>::type() {
    return kuRaster_type_s32;
}
//----------------------------------------
template <>
int kuRaster_<f32>::type() {
    return kuRaster_type_f32;
}
//----------------------------------------
template <>
int kuRaster_<u16>::type() {
    return kuRaster_type_u16;
}

//----------------------------------------
