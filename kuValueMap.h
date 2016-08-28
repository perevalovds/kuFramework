#pragma once

//Таблица ключ-значение

#include <kuMain.h>
#include <map>

class kuValueMap {
public:
    kuValueMap() {
	}

    ~kuValueMap() {
	}

    void save( const string &fileName );
    void load( const string &fileName );

    typedef map<string, string, less<string> > StringStringMap;
    typedef StringStringMap::value_type StringStringMapValueType;

    typedef map<string, int, less<string> > StringIntMap;
    typedef StringIntMap::value_type StringStringIntValueType;

    vector<string> keys(); //unsorted!

	void deleteKey( string key )	{
		_values.erase( key );
	}

	void clear() {
		_values.clear();
	}

	bool keyExists( const string &key ) {
		StringStringMap::iterator p = _values.find( key );
		return ( p != _values.end() );		
	}

	string getString( const string &key, string def = "" ) {
		StringStringMap::iterator p = _values.find( key );
		if (p != _values.end()) {
			return p->second;
		}
		else {
			return def;
		}
	}

	int getInt( const string &key, int def = 0 ) {
		string value = getString( key );
        if ( value != "" ) return kuToInt( value);
					 else return def;
	}

	double getDouble( const string &key, double def = 0 ) {
		string value = getString( key );
        if ( value != "" ) return kuToDouble( value );
					 else return def;
	}
	float getFloat( const string &key, float def = 0 ) {
		return getDouble( key, def );
	}

	bool getBool( const string &key, bool def = false ) {
		string value = getString( key );
		if ( value != "" ) {
			if ( value == "1" ) return true;
							else return false;
		}
		 else return def;
	}

	void setString( const string &key, const string &value ) {
		//_values.insert( StringStringMapValueType( key, value) );
        if ( keyExists(key) ) { deleteKey( key ); }
		_values[key] = value;
	}

	void setDouble( const string &key, double dValue )
	{
        setString( key, kuToString( dValue ) );
	}

	void setFloat( const string &key, float value )
	{
		setDouble( key, value );
	}

	void setInt( const string &key, int iValue )
	{
        setString( key, kuToString( iValue ) );
	}
	void setBool( const string &key, bool bValue )
	{
		string value;
		if (bValue) value = "1";
		else value = "0";
		setString( key, value );
	}

protected:
	StringStringMap _values;


};



