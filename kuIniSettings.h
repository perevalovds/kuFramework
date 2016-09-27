#pragma once

#include <map>

#include "kuMain.h"

class kuIniSettings {
public:

	kuIniSettings() {}
	kuIniSettings(string filename) { load(filename); }

	typedef map<string, string> StringStringMap;

    bool load( string filename, bool clearFirst=true, bool verbose = false );
    bool loadSilent( string filename, bool clearFirst=true ) { return load(filename, clearFirst, false); }
    bool has(string key) { return keys[key]!=""; }
    void clear();

    vector<string> getKeys() {
        vector<string> _keys;

        for (StringStringMap::const_iterator iter = keys.begin();
            iter != keys.end(); iter++) {
            string key = iter->first;
            _keys.push_back( key );
        }
        return _keys;
    }

    vector<string> getValues() {
        vector<string> _keys;
        typedef map<string, string> StringStringMap;

        for (StringStringMap::const_iterator iter = keys.begin();
            iter != keys.end(); iter++) {
            string key = iter->second;
            _keys.push_back( key );
        }
        return _keys;
    }
    void deleteKey( string key )	{
		keys.erase( key );
    }
    bool keyExists( const string &key ) {
        StringStringMap::iterator p = keys.find( key );
        return ( p != keys.end() );
    }

    void setString( const string &key, const string &value ) {
        if ( keyExists(key) ) { deleteKey( key ); }
		keys[key] = value;
    }

    map<string,string> keys;
    string outputFilename;

    //getters
    int getInt(string key, bool required = true, int defaultValue = 0 );
    float getFloat(string key, bool required = true, float defaultValue = 0 );
    ldouble getLongDouble(string key, bool required = true, ldouble defaultValue = 0 );
    string getString(string key, bool required = true, string defaultValue = "" );


    string filename_;
protected:

};


