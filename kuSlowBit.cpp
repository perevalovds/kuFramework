#include "kuSlowBit.h"


//--------------------------------------------------------------
kuSlowBit::kuSlowBit()
{
    speed00 = speed01 = speed10 = speed11 = 1.0;
    _state = 0;
    _lastState = _state;
    _value = _state;
}

//--------------------------------------------------------------
void kuSlowBit::setup( string line, string title0 )
{
	vector<float> list = kuSplitStringf( line, "," );
    kuAssert( list.size() >= 4,
                     "Bad or empty slowbit '" + title0
                     + "' string '" + line + "'" );
    
    for (int i=0; i<list.size(); i++) {
        if ( list[i] < 0.000001 ) {
            list[i] = 0.000001;
        }
    }
    speed00 = 1.0 / list[0];
    speed01 = 1.0 / list[1];
    speed10 = 1.0 / list[2];
    speed11 = 1.0 / list[3];

    _state = 0;
    _lastState = _state;
    _value = _state;
    
    title = title0;
}

//--------------------------------------------------------------
void kuSlowBit::reset0()
{
    reset( 0 );
}

//--------------------------------------------------------------
void kuSlowBit::reset( int value )
{
    _state = value;
    _lastState = _state;
    _value = _state;
}

//--------------------------------------------------------------
void kuSlowBit::update( float dt, int target ) //target - целевое состояние в которое сейчас надо идти
{
    _lastState = _state;
    
    float d = 0;
    if ( _state == 0 ) {
        if ( target == 0 ) { d = -speed00; }
        else { d = speed01; }
    }
    else {
        if ( target == 0 ) { d = -speed10; }
        else { d = speed11; }
    }
    
    _value += d * dt;
    if ( _value >= 1 ) { _state = 1; }
    if ( _value <= 0 ) { _state = 0; }
    
    _value = kuClamp( _value, 0, 1 );
}

//--------------------------------------------------------------
int kuSlowBit::state()       //текущее состояние
{
    return _state;
}

//--------------------------------------------------------------
bool kuSlowBit::became0()     //изменился в 0
{
    return ( changed() && _state == 0 );
}

//--------------------------------------------------------------
bool kuSlowBit::became1()     //изменился в 1
{
    return ( changed() && _state == 1 );
}

//--------------------------------------------------------------
bool kuSlowBit::changed()     //изменился (сравнивать с предыдущим состоянием)
{
    return ( _state != _lastState );
}

//--------------------------------------------------------------
void kuSlowBit::draw( float x, float y, float w, float h ) {
/*    if ( state() == 0 ) ofSetColor(64,64,255);
    else ofSetColor(64,200,64);
    ofNoFill();
    ofDrawRectangle(x, y, w, h);
    ofFill();
    ofDrawRectangle(x, y, w*stateFloat(), h);
    if( title!="") {
        ofSetColor(128);
        ofDrawBitmapString(title, x+5, y+12);
    }
  */  
}

//--------------------------------------------------------------
