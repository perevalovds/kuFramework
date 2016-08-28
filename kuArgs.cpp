#include "kuArgs.h"
#include "kuLog.h"

//------------------------------------------------------
kuArgs::kuArgs(){
}

//------------------------------------------------------
void kuArgs::setup( int argc, char* argv[] ) {
	this->argc = argc;

	string cVal, nextVal;
	for(int i=0; i < argc; i++){
		cVal = argv[i];
		nextVal = (i < argc-1)?argv[i+1]:"";
		
        size_t pos = cVal.find('=');
		if(pos != string::npos){
			string key = cVal.substr(0, pos);
			string value = cVal.substr(pos+1);
			this->opts[key] = value;
			this->args.push_back(key);
			this->args.push_back(value);
			this->argc++;
		}
		else{
			this->args.push_back(cVal);
			if(nextVal != "" && cVal.substr(0, 1) == "-" && nextVal.substr(0, 1) != "-" && nextVal.find('=') == string::npos){
				this->opts[cVal] = nextVal;
				this->args.push_back(nextVal);
				i++;
			}
		}
	}
}

//------------------------------------------------------
int kuArgs::getCount(){
	return this->argc;
}
//------------------------------------------------------
bool kuArgs::contains(string key){
	for(int i = 0; i < this->argc; i++){
		if(this->args[i] == key){
			return true;
		}
	}
	return false;
}

//------------------------------------------------------
float kuArgs::getFloat(string key, float def){
	map<string,string>::iterator it;
	it = this->opts.find(key);
	if(it != this->opts.end()){
		return strToFloat(it->second);
	}
	return def;
}

//------------------------------------------------------
int kuArgs::getInt(string key, int def){
	map<string,string>::iterator it;
	it = this->opts.find(key);
	if(it != this->opts.end()){
		return strToInt(it->second);
	}
	return def;
}

//------------------------------------------------------
string kuArgs::getString(string key, string def){
	map<string,string>::iterator it;
	it = this->opts.find(key);
	if(it != this->opts.end()){
		return it->second;
	}
	return def;
}

//----------------------------------------------------------------
float kuArgs::getFloat(int index){
	if(index < this->argc){
		return strToFloat(this->args[index]);
	}
	return 0.0f;
}

//------------------------------------------------------
int kuArgs::getInt(int index){
	if(index < this->argc){
		return strToInt(this->args[index]);
	}
	return 0;
}

//------------------------------------------------------
string kuArgs::getString(int index){
	if(index < this->argc){
		return this->args[index];
	}
	return "";
}

//------------------------------------------------------
void kuArgs::printArgs(){
	kuLog << "--- arguments --------" << endl;
	for(int i=0; i < this->argc; i++)
        kuLog << i << " => " << this->args[i].c_str() << endl;
}

//------------------------------------------------------
void kuArgs::printOpts(){
	map<string,string>::iterator it;
	kuLog << "--- options ----------" << endl;
	for ( it=opts.begin() ; it != opts.end(); it++ )
        kuLog << (*it).first.c_str() << " => " << (*it).second.c_str() << endl;
}

//------------------------------------------------------
int kuArgs::strToInt(string value){
	int result;
	sscanf(value.c_str(), "%d", &result);
	return result;
}

//------------------------------------------------------
float kuArgs::strToFloat(string value){
	float result;
	sscanf(value.c_str(), "%f", &result);
	return result;
}

//------------------------------------------------------
