#pragma once

//kuSlowBit - бит, который переключается из 0 в 1 и 1 в 0 постепенно.

#include "kuMain.h"


struct kuSlowBit {
    kuSlowBit();
    //4 числа, разделенные запятыми, времена переходов
    //0->0, 0->1, 1->0, 1->1,
    //например, "1.0, 0.1, 1.0, 0.1"
    void setup( string line, string title="" );
	void setup( float time00, float time01, float time10, float time11, string title0 = "" );
    void update( float dt, int target ); //target - целевое состояние в которое сейчас надо идти
    void update_times( float time00, float time01, float time10, float time11 );

    int state();       //текущее состояние
    float stateFloat() { return _value; }   //переходное значение
    bool became0();     //изменился в 0
    bool became1();     //изменился в 1
    bool changed();     //изменился (сравнивать с предыдущим состоянием)

    void reset0();
    void reset( int value );
    
    string title;
    void draw( float x, float y, float w, float h );

private:
    string _name;
    //скорости перехода для _value, берутся из ini
    float speed01, speed00, speed10, speed11;

    int _state;     //состояние
    int _lastState;
    float _value;   //плавно переходящее значение

};


