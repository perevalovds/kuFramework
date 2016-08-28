#pragma once

//Командная строка

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class kuArgs {
public:
    kuArgs();
	void setup( int argc, char* argv[] );
	
	int getCount();
	
	bool contains(string key);

	float getFloat( string key, float def = 0.0 );
	int getInt( string key, int def = 0 );
	string getString( string key, string def = "" );
	
	float getFloat(int index);
	int getInt(int index);
	string getString(int index);

	void printArgs();
	void printOpts();
private:
	int argc;
	vector<string> args;
	map<string,string> opts;
	
	int strToInt(string value);
	float strToFloat(string value);
};


