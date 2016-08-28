#pragma once

#include <map>

#include "kuMain.h"

class kuIniSettings {
public:

    kuIniSettings() {}
    kuIniSettings(string filename) { load(filename); }

    typedef map<string, string, less<string> > StringStringMap;

    bool load( string filename, bool clearFirst=true, bool verbose = false );
    bool loadSilent( string filename, bool clearFirst=true ) { return load(filename, clearFirst, false); }
    bool has(string key) { return keys[key]!=""; }
    void clear();

    vector<string> getKeys() {
        vector<string> _keys;
        typedef map<string, string> StringStringMap;

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
        _values.erase( key );
    }
    bool keyExists( const string &key ) {
        StringStringMap::iterator p = _values.find( key );
        return ( p != _values.end() );
    }

    void setString( const string &key, const string &value ) {
        //_values.insert( StringStringMapValueType( key, value) );
        if ( keyExists(key) ) { deleteKey( key ); }
        _values[key] = value;
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
    StringStringMap _values;
};


