#include "kuIniSettings.h"

static istream& crossplatform_getline(istream& is, string& str) {
    char ch;

    str.clear();

    while (is.get(ch)) {
        if(ch == '\n') {
            break;
        } else if(ch == '\r') {
            if(is.get(ch)) {
                if(ch != '\n') {
                    is.unget();
                }
            }
            break;
        }

        str += ch;
    }
    return is;
}

bool kuIniSettings::load(string filename, bool clearFirst, bool verbose ) {
    if ( verbose ) kuLog << "\tloading ini " << filename << endl;
    filename_ = filename;
    //filename = ofToDataPath(filename);
    kuAssert( kuFileExists(filename), "ini file \"" + filename + "\" doesn't exists" );

    string cmd,section,key,value,id;
    ifstream f(filename.c_str(),ios::in);

    if (clearFirst) clear();

    while (crossplatform_getline(f,cmd)) {
		if ( !cmd.empty() ) {
            switch (cmd[0]) {
	         case 0: break;   //empty first line?
	         case '#': break; //comment
	         case ';': break; //comment
	         case '[': 
					{
                        size_t right = cmd.find( ']' );
						if ( right != string::npos ) {
                            section = cmd.substr(1,int(right)-1); break; //section, remove brackets
						}
					}
				default:
                 size_t pos = cmd.find("=", 0);
	             if (pos==string::npos) break;
                 key = cmd.substr(0, pos);
                 value = cmd.substr(pos+1);
	             id = section!="" ? (section + "." + key) : key;
	             keys[id] = value;
	             //kuLog << "ini: " << id << " = " << value << endl;
	     }
   
		}
	}
    f.close();
    return true;
}

//-----------------------------------------------------------------------
void kuIniSettings::clear() {
    keys.clear();
}

//-----------------------------------------------------------------------
//если есть значение по умолчанию, мы не выдаём ошибку если не найдено значение
//- для надёжной работы нужно чтобы всегда были значения
string kuIniSettings::getString(string key, bool required, string defaultValue) {
    if ( required ) {
        kuAssert( has(key), "no string value '" + key +"' in ini-file " + filename_ );
    }
    return ( has(key) ) ? keys[key] : defaultValue;
}

//-----------------------------------------------------------------------
int kuIniSettings::getInt(string key, bool required, int defaultValue) {
    if ( required ) {
        kuAssert( has(key), "no int value '" + key +"' in ini-file " + filename_ );
    }
    return ( has(key) ) ? kuToInt(keys[key]) : defaultValue;
}

//-----------------------------------------------------------------------
float kuIniSettings::getFloat(string key, bool required, float defaultValue) {
    if ( required ) {
        kuAssert( has(key), "no float value '" + key +"' in ini-file " + filename_ );
    }
    return ( has(key) ) ? kuToFloat(keys[key]) : defaultValue;
}

//-----------------------------------------------------------------------
ldouble kuIniSettings::getLongDouble(string key, bool required, ldouble defaultValue ) {
    if ( required ) {
        kuAssert( has(key), "no double value '" + key +"' in ini-file " + filename_ );
    }
    return ( has(key) ) ? kuToFloat(keys[key]) : defaultValue;
}

//-----------------------------------------------------------------------

