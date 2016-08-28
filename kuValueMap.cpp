
#include "kuValueMap.h"

//---------------------------------------------------------------------------
void kuValueMap::save( const string &fileName ) {
    vector<string> key = keys();
    size_t n = key.size();
    vector<string> file(n);
    for (size_t i=0; i<n; i++) {
        file[i] = key[i] + "=" + getString(key[i]);
    }
    kuFileWriteStrings( file, fileName );
}

//---------------------------------------------------------------------------
void kuValueMap::load( const string &fileName ) {
    vector<string> file = kuFileReadStrings( fileName );
    size_t n = file.size();
    for (size_t i=0; i<n; i++) {
        string &line = file[i];
        size_t pos = line.find( '=', 0 );
        if ( pos != string::npos && pos > 0 ) {
            string key = line.substr(0, pos);
            string value = line.substr(pos+1);
            setString( key, value );
        }
    }
}
//---------------------------------------------------------------------------
vector<string> kuValueMap::keys()
{
	vector<string> _keys;
	for (StringStringMap::const_iterator iter = _values.begin();
		iter != _values.end(); iter++) {
		string key = iter->first;
		_keys.push_back( key );
	}
	return _keys;
}

//---------------------------------------------------------------------------

