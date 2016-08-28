#pragma once

#include "kuConstants.h"

//--------------------------------------------------
//Поиск, сортировка

template<class T, class BoolFunction>
void kuRemove(vector<T>& values, BoolFunction shouldErase) {
	values.erase(remove_if(values.begin(), values.end(), shouldErase), values.end());
}

template<class T>
void kuSort(vector<T>& values) {
	sort(values.begin(), values.end());
}
template<class T, class BoolFunction>
void kuSort(vector<T>& values, BoolFunction compare) {
	sort(values.begin(), values.end(), compare);
}

void kuSortInt( vector<int>& values );
void kuSortFloat( vector<float>& values );
void kuSortDouble( vector<double>& values );

int kuCompareU8_pointers(const void * a, const void * b);
int kuCompareInt_pointers(const void * a, const void * b);
int kuCompareFloat_pointers(const void * a, const void * b);

//Возвращает индекс найденного элемента или размер вектора если элемента нет
template <class T>
unsigned int kuFind(const vector<T>& values, const T& target) {
	return distance(values.begin(), find(values.begin(), values.end(), target));
}

template <class T>
bool kuContains(const vector<T>& values, const T& target) {
    return kuFind(values, target) != values.size();
}

//--------------------------------------------------
//Конвертация типов
template <class T>
string kuToString(const T& value){
	ostringstream out;
	out << value;
	return out.str();
}

template <class T>
string kuToString(const T& value, int precision){
	ostringstream out;
	out << fixed << setprecision(precision) << value;
	return out.str();
}

template <class T>
string kuToString(const T& value, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << value;
	return out.str();
}

template <class T>
string kuToString(const T& value, int precision, int width, char fill ){
	ostringstream out;
	out << fixed << setfill(fill) << setw(width) << setprecision(precision) << value;
	return out.str();
}

template<class T>
string kuToString(const vector<T>& values) {
	stringstream out;
	int n = values.size();
	out << "{";
	if(n > 0) {
		for(int i = 0; i < n - 1; i++) {
			out << values[i] << ", ";
		}
		out << values[n - 1];
	}
	out << "}";
	return out.str();
}

int kuToInt(const string& intString);
int kuToInt(const string& intString, int defValue );    //defValue используется если intString пустая
u32 kuToUInt32(const string& intString);
u8 kuToU8(const string& intString);

char kuToChar(const string& charString);
char kuToChar(const string& charString, char defValue);
float kuToFloat(const string& floatString);
float kuToFloat(const string& floatString, float defValue);
double kuToDouble(const string& doubleString);
double kuToDouble(const string& doubleString, double defValue);
ldouble kuToLDouble(const string& ldoubleString );
ldouble kuToLDouble(const string& ldoubleString, ldouble defValue);

//--------------------------------------------------
//Обработка строк
vector <string> kuSplitString (const string & source, const string & delimiter, bool ignoreEmpty = false );
vector <float> kuSplitStringf (const string & source, const string & delimiter, bool ignoreEmpty = false );
vector <double> kuSplitStringd (const string & source, const string & delimiter, bool ignoreEmpty = false );
vector <int> kuSplitStringi (const string & source, const string & delimiter, bool ignoreEmpty = false );


string kuJoinString(vector <string> stringElements, const string & delimiter);
bool kuStringReplace(string& input, string searchStr, string replaceStr);
void kuStringRemovePathSymbols( string& input );    //удаление из строки символов : . \\ / ? *
bool kuIsStringInString(string haystack, string needle);

//Убрать символы " ", "\t", "\n" по краям строки
void kuStringTrim(string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);
void kuStringTrimLeft(string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);
void kuStringTrimRight(string& input, bool trimSpace = true, bool trimTab = true, bool trimBSlashN = true);

string kuToLower(const string & src);
string kuToUpper(const string & src);

bool kuStringBeginsWith( const string &s, const string &prefix );
bool kuStringEndsWith(const string &s, const string &suffix );
char kuStringLastSymbol( const string &s );

bool kuStringIsInt( const string &s );  //является ли строка целым числом

//--------------------------------------------------
//Файлы
bool kuFileExists( string filename );
u64 kuFileSize( string fileName );
string kuFileRemoveExtension( string fileName );    //имя файла без расширения
string kuFileGetExtension( string fileName );         //расширение файла
string kuFileGetName(string file );        //убирает из имени файла путь
string kuFileGetPath( string file );   //возвращает путь к файлу, без имени
string kuFileGetAbsolutePath( string file );   //возвращает полный путь к файлу

vector<string> kuFilesFind( string path, string fileMask );  //поиск файлов по маске

FILE *cmFileAppendBinary( string fileName );  //открыть для дописывания
void kuFileAppendText( const string &fileName, const string &text );

FILE *cmFileReadPos( string fileName, u64 pos );  //открыть для чтения
FILE *cmFileUpdatePos( string fileName, u64 pos );  //открыть для обновления части файла

void kuFileSaveArray( FILE *file, void *arr, size_t size );  //запись массива в файл, блоками
void kuFileLoadArray( FILE *file, void *arr, size_t size );  //считывание массива из файла, блоками

vector<string> kuFileReadStrings( string fileName );    //считать текстовый файл в строки
string kuFileReadString( string fileName );    //считать текстовый файл в строку, с контролем \r для Linux

void kuFileWriteStrings( const vector<string> &list, string fileName );    //записать строки в текстовый файл
void kuFileWriteString( const string &line, const string &fileName );    //записать строку в текстовый файл

int kuFileReadInt( string fileName );
void kuFileWriteInt( int value, string fileName );

string kuFetchExePath(); //путь к исполняемому файлу

string kuPathToWindows( string path ); //сконвертировать путь для Windows
string kuPathToLinux( string path ); //сконвертировать путь для Linux)
string kuPathToQt( string path ); //сконвертировать путь для Qt
string kuPathToOS( string path ); //сконвертировать путь по ОС (\ в Windows, / в Linux)
bool kuMakeDir( string path );  //создать папку
bool kuDirRename( string from, string to ); //переименование папки
bool kuDirDelete( string path );   //удаление папки, если существует
bool kuDirExists( string path );  //существует ли папка
string kuDirGetAbsolutePath( string dir0 );   //возвращает полный путь к папке, без '~'
u64 kuDirSize( string path );    //Место, занимаемое файлами папки

bool kuCd( string path );       //перейти в каталог
bool kuFileCopy( string from, string to );  //копирование файла
bool kuFileCopyProgress ( string source, string dest, float progress0, float progress1 ); // копирование файлов с PROGRESS
bool kuFileDelete( string path );   //удаление файла

u64 kuDiskFreeSpace(); //Cвободное место на диске, из которого запущена программа

//--------------------------------------------------
//Строковые буферы
void kuStringToBuffer( string s, u8 *buffer, int bufferSize );
string kuBufferToString( u8 *buffer, int bufferSize );

//--------------------------------------------------
//Время

void kuSetup(); //инициализация времени, и других подсистем
int kuElapsedSecondsi();    //число секунд от старта приложения
void kuSleepMs(int msec);  //сон
string kuTimeString();  //текущее время, в виде строки

//Определение типа
bool kuIsInt(string s);
bool kuIsFloat(string s);



