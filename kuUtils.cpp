#include "kuUtils.h"
#include "kuLog.h"

/*#include <QString>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QEventLoop>
#include <QDirIterator>
#include <qdatetime.h>
*/

#include <iomanip>
//#include <windows.h>
#include <fstream>
#include <iostream>
#include "kuMath.h"


#ifdef OS_WINDOWS
#include <Windows.h>
#endif


//----------------------------------------
int kuCompareU8_pointers(const void * a, const void * b) {
    return ( *(u8*)a - *(u8*)b );
}

//----------------------------------------
int kuCompareInt_pointers(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//----------------------------------------
int kuCompareFloat_pointers(const void * a, const void * b) {
    return ( *(float*)a - *(float*)b );
}

//----------------------------------------
int kuCompareInt(int a, int b ) {
    return ( a - b );
}

//----------------------------------------
int kuCompareFloat( float a, float b ) {
    return ( a - b );
}

//----------------------------------------
int kuCompareDouble( float a, float b ) {
    return ( a - b );
}

//----------------------------------------
void kuSortInt( vector<int>& v ) {
    kuSort( v, kuCompareInt );
}

//----------------------------------------
void kuSortFloat( vector<float>& v ) {
    kuSort( v, kuCompareFloat );
}

//----------------------------------------
int ku_compare_double (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}

//----------------------------------------
void kuSortDouble( vector<double>& values ) {
    qsort( &values[0], values.size(), sizeof(values[0]), ku_compare_double );
    //kuSort( v, kuCompareDouble );
}

//----------------------------------------
int kuToInt(const string& intString) {
	int x = 0;
	istringstream cur(intString);
	cur >> x;
	return x;
}

//----------------------------------------
u32 kuToUInt32(const string& intString) {
    u32 x = 0;
    istringstream cur(intString);
    cur >> x;
    return x;
}
//---------------------------------------
u8 kuToU8(const string& intString) {
    u8 x = 0;
    istringstream cur(intString);
    cur >> x;
    return x;
}

//----------------------------------------
float kuToFloat(const string& floatString) {
	float x = 0;
	istringstream cur(floatString);
	cur >> x;
	return x;
}

//----------------------------------------
double kuToDouble(const string& doubleString) {
	double x = 0;
	istringstream cur(doubleString);
	cur >> x;
	return x;
}

//----------------------------------------
ldouble kuToLDouble( const string& ldoubleString ) {
    ldouble x = 0;
    istringstream cur(ldoubleString);
    cur >> x;
    return x;
}


//----------------------------------------
char kuToChar(const string& charString) {
	char x = '\0';
	istringstream cur(charString);
	cur >> x;
	return x;
}

//----------------------------------------
int kuToInt(const string& intString, int defValue ) {
    return (intString.empty()) ? defValue : kuToInt( intString );
}

//----------------------------------------
char kuToChar(const string& charString, char defValue) {
    return (charString.empty()) ? defValue : kuToChar( charString );
}

//----------------------------------------
float kuToFloat(const string& floatString, float defValue) {
    return (floatString.empty()) ? defValue : kuToFloat( floatString );
}

//----------------------------------------
double kuToDouble(const string& doubleString, double defValue) {
    return (doubleString.empty()) ? defValue : kuToDouble( doubleString );
}

//--------------------------------------------------
ldouble kuToLDouble(const string& ldoubleString, ldouble defValue) {
    return (ldoubleString.empty()) ? defValue : kuToLDouble( ldoubleString );
}

//--------------------------------------------------
vector <string> kuSplitString(const string &source, const string &delimiter, bool ignoreEmpty) {
	vector<string> result;
	if (delimiter.empty()) {
		result.push_back(source);
		return result;
	}
	string::const_iterator substart = source.begin(), subend;
	while (true) {
		subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
		string sub(substart, subend);
		if (!ignoreEmpty || !sub.empty()) {
			result.push_back(sub);
		}
		if (subend == source.end()) {
			break;
		}
		substart = subend + delimiter.size();
	}
	return result;
}

//--------------------------------------------------
vector <float> kuSplitStringf (const string & source, const string & delimiter, bool ignoreEmpty) {
    vector<string> list = kuSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<float> res(n);
    for (int i=0; i<n; i++) {
        res[i] = kuToFloat( list[i] );
    }
    return res;
}

//--------------------------------------------------
vector <double> kuSplitStringd (const string & source, const string & delimiter, bool ignoreEmpty) {
    vector<string> list = kuSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<double> res(n);
    for (int i=0; i<n; i++) {
        res[i] = kuToDouble( list[i] );
    }
    return res;
}

//--------------------------------------------------
vector <int> kuSplitStringi (const string & source, const string & delimiter, bool ignoreEmpty ) {
    vector<string> list = kuSplitString( source, delimiter, ignoreEmpty );
    int n = list.size();
    vector<int> res(n);
    for (int i=0; i<n; i++) {
        res[i] = kuToInt( list[i] );
    }
    return res;
}

//--------------------------------------------------
string kuJoinString(vector <string> stringElements, const string & delimiter){
	string resultString = "";
	int numElements = stringElements.size();

	for(int k = 0; k < numElements; k++){
		if( k < numElements-1 ){
			resultString += stringElements[k] + delimiter;
		} else {
			resultString += stringElements[k];
		}
	}

	return resultString;
}

//--------------------------------------------------
bool kuStringReplace(string& input, string searchStr, string replaceStr){
	size_t uPos = 0; 
	size_t uFindLen = searchStr.length(); 
	size_t uReplaceLen = replaceStr.length();
		
    bool found = false;
	if( uFindLen == 0 ){
        return found;
	}

	for( ;(uPos = input.find( searchStr, uPos )) != std::string::npos; ){
		input.replace( uPos, uFindLen, replaceStr );
		uPos += uReplaceLen;
        found = true;
	}	
    return found;
}

//--------------------------------------------------
void kuStringTrim(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
    kuStringTrimLeft( input, trimSpace, trimTab, trimBSlashN );
    kuStringTrimRight( input, trimSpace, trimTab, trimBSlashN );
}

//--------------------------------------------------
void kuStringTrimLeft(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
    while (!input.empty()) {
        char c = input[0];
        if ( (trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r') ) ) {
            input = input.substr(1);
        }
        else break;
    }
}

//--------------------------------------------------
void kuStringTrimRight(string& input, bool trimSpace, bool trimTab, bool trimBSlashN ) {
     while (!input.empty()) {
        int i=int(input.length()-1);
        char c = input[i];
        if ( (trimSpace && c == ' ') || (trimTab && c == '\t') || (trimBSlashN && (c == '\n' || c == '\r') ) ) {
            input = input.substr(0, input.length()-1);
        }
        else break;
    }
}

//--------------------------------------------------
void kuStringRemovePathSymbols( string& input )    //удаление из строки символов : . \\ / ? *
{
    kuStringReplace( input, ":", "" );
    kuStringReplace( input, ".", "" );
    kuStringReplace( input, "\\", "" );
    kuStringReplace( input, "/", "" );
    kuStringReplace( input, "?", "" );
    kuStringReplace( input, "*", "" );

}

//--------------------------------------------------
bool kuIsStringInString(string haystack, string needle){
	return ( strstr(haystack.c_str(), needle.c_str() ) != NULL );
}

//--------------------------------------------------
string kuToLower(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::tolower);
	return dst;
}

//--------------------------------------------------
string kuToUpper(const string & src){
	string dst(src);
	transform(src.begin(),src.end(),dst.begin(),::toupper);
	return dst;
}

//--------------------------------------------------
bool kuStringBeginsWith( const string &s, const string &prefix ) {
    return ( s.substr(0, prefix.length()) == prefix );
}

//--------------------------------------------------
bool kuStringEndsWith( const string &s, const string &suffix ) {
    return ( s.length() >= suffix.length() && s.substr(s.length()-suffix.length(), suffix.length()) == suffix );
}

//--------------------------------------------------
char kuStringLastSymbol( const string &s ) {
    if ( !s.empty() ) return s[ s.length()-1 ];
    else return 0;
}

//--------------------------------------------------
bool kuStringIsInt( const string &s ) {  //является ли строка целым числом
    string s1 = s;
    kuStringTrim( s1 );
    for (size_t i=0; i<s1.length(); i++) {
        char c = s1[i];
        if ( !((c >= '0' && c <= '9') || (i==0 && (c == '+' || c == '-')) ) ) {
            return false;
        }
    }
    return true;
}

//--------------------------------------------------
bool kuFileExists( string filename ) {
	ifstream inp;
	inp.open(filename.c_str(), ifstream::in);
    //inp.close();
	return !inp.fail();
}

//--------------------------------------------------
u64  kuFileSize( string fileName ) {
    //struct stat stat_buf;
    //int rc = stat( fileName.c_str(), &stat_buf );
    //return rc == 0 ? stat_buf.st_size : 0; //-1;

	//64 bit
    //u64 size = 0;
    //FILE *file = fopen( fileName.c_str(),"rb" );
    //if ( file ) {
    //  fseeko64( file, 0, SEEK_END );
    //    //TODO контроль того что ошибка
    //   size = ftello64( file );
    //    fclose( file );
    //}
    //return size;

	//32 bit
	u64 size = 0;
	FILE *file = fopen( fileName.c_str(), "rb" );
	if ( file ) {
		fseek( file, 0, SEEK_END );
		size = ftell( file );	
		fclose( file );
	}
	return size;

}

//--------------------------------------------------
string kuFileRemoveExtension( string fileName )    //имя файла без расширения
{
    if ( !fileName.empty() ) {
        int n = fileName.length();
        int k = kuFileGetExtension( fileName ).length();
        return fileName.substr( 0, n-k-1 );
    }
    return "";
}

//--------------------------------------------------
string kuFileGetExtension( string fileName )         //расширение файла
{
    if ( !fileName.empty() ) {
        int n = fileName.size();
        for (int i=n-1; i>=0; i--) {
            unsigned char c = fileName[i];
            if ( c == '.' ) {
                return fileName.substr(i+1);
            }
            if ( c == '/' || c == '\\' ) break;
        }
    }
    return "";
}

//--------------------------------------------------
/*string kuFileGetName( string file ) {   //убирает из имени файла путь
    QFileInfo fileInfo( file.c_str() );
    QString filename(fileInfo.fileName());
    return filename.toUtf8().constData();
}

//--------------------------------------------------
string kuFileGetPath(string file ) {   //возвращает путь к файлу, без имени
    QFileInfo fileInfo( file.c_str() );
    QString path(fileInfo.path());
    return path.toUtf8().constData();
}

//--------------------------------------------------
string kuFileGetAbsolutePath( string file ) {   //возвращает полный путь к файлу
    QFileInfo fileInfo( file.c_str() );
    QString path(fileInfo.absolutePath());
    return path.toUtf8().constData();
}

//--------------------------------------------------
string kuDirGetAbsolutePath( string dir0 ) {   //возвращает полный путь к папке, без '~'
    QDir dir( dir0.c_str() );
    QString path(dir.absolutePath());//canonicalPath());
    return path.toUtf8().constData();
}

//--------------------------------------------------
u64 kuDirSize( string path ) {   //Место, занимаемое файлами папки
    //vector<string> files = kuFilesFind( path, "*");

    u64 size = 0;

    QDirIterator it(path.c_str(), QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        size += it.fileInfo().size();
        it.next();
    }
    return size;
}

//--------------------------------------------------
vector<string> kuFilesFind(string path, string fileMask) {  //поиск файлов по маске
    path = kuPathToQt( path );

    QDir currentDir(path.c_str());
    QString fileMaskqs = fileMask.c_str();
    if (fileMaskqs.isEmpty()) fileMaskqs = "*";
    QStringList files = currentDir.entryList(QStringList(fileMaskqs), QDir::Files | QDir::NoSymLinks);
    size_t n = files.size();
    vector<string> list(n);
    for (size_t i=0; i<n; i++) {
        list[i] = files[i].toUtf8().constData();
    }
    return list;
}*/

//--------------------------------------------------
/*FILE *cmFileAppendBinary( string fileName )  //открыть для дописывания
{
    FILE *file = fopen( fileName.c_str(),"a+b" );
    fseeko64( file, 0, SEEK_END );
    return file;
}*/

//----------------------------------------
void kuFileAppendText( const string &fileName, const string &text ) {
    ofstream file(fileName.c_str(), ios::out | ios::app);
    file << text <<endl;
    file.close();
}

//--------------------------------------------------
/*FILE *cmFileReadPos( string fileName, u64 pos )  //открыть для чтения
{
	FILE *file = fopen( fileName.c_str(),"rb" );
    fseeko64( file, pos, SEEK_SET );
    return file;
}*/

//--------------------------------------------------
/*
FILE *cmFileUpdatePos( string fileName, u64 pos )  //открыть для обновления части файла
{
    FILE *file = fopen( fileName.c_str(),"r+b" );
    fseeko64( file, pos, SEEK_SET );
    return file;
}
*/

//--------------------------------------------------
void kuFileSaveArray( FILE *file, void *arr, size_t size )  { //запись массива в файл, блоками
    u8 *data = (u8 *)arr;
    //fwrite( arr, 1, size, file );
    size_t block = 50 * 1000 * 1000;   //размер блока
    bool print = ( size > 2*block );
    size_t k = 0;
    while ( k < size ) {
        size_t k1 = min( k + block, size );
        fwrite( data + k, 1, k1 - k, file );
        kuAssert( !ferror(file), "cmFileSaveArray - error saving to file, no memory on disk? (attempt to write "
                  + kuToString( size ) + " bytes" );

        k = k1;
        if ( print ) {
            kuLog << " " << int(double(k) / size * 100)<< "%" << endl;
        }
    }

}

//--------------------------------------------------
void kuFileLoadArray( FILE *file, void *arr, size_t size ) {  //считывание массива из файла, блоками
    u8 *data = (u8 *)arr;
    //fwrite( arr, 1, size, file );
    size_t block = 100 * 1000 * 1000;   //размер блока
    bool print = ( size > 2*block );
    size_t k = 0;
    while ( k < size ) {
        size_t k1 = min( k + block, size );
        fread( data + k, 1, k1 - k, file );
        kuAssert( !ferror(file), "cmFileLoadArray - error loading from file" );

        k = k1;
        if ( print ) {
            kuLog << " " << int(double(k) / size * 100) << "%" << endl;
        }
    }
    //if ( print ) {
    //    kuLog << endl;
    //}
}

//--------------------------------------------------
/*bool kuFileCopyProgress( string source, string dest, float progress0, float progress1 ) { // копирование файлов с PROGRESS
    if (!kuFileExists(source)) return false;

    u64 size = kuFileSize( source );
    //cout << "MESSAGE_DLG: " << " file size " << size << endl;

    FILE *inFile = fopen(source.c_str(), "rb");
    kuAssert(inFile, "Cannot open " + source);

    FILE *outFile = fopen(dest.c_str(), "wb");
    if(outFile == 0)
        fclose(inFile);

    kuAssert(outFile, "Cannot open " + dest);

    u64 block = 100 * 1000 * 1000;
    vector<u8> data( block );
    u64 k = 0;

    bool res = true;
    while (k < size) {
        u64 k1 = min(k + block, size);
        fread(&data[0], 1, k1 - k, inFile);
        fwrite(&data[0], 1, k1 - k, outFile);
        //kuAssert(!ferror(inFile), "cmCopyFileProgress - error loading from file");
        if ( ferror(inFile) ) { res = false; break; }

        int progress = int( kuMap( double(k1)/double(size), 0, 1, progress0, progress1 ) + 0.5 );
        kuLogProgress << progress << endl;
        k = k1;
    }

    fclose(inFile);
    fclose(outFile);
    return res;
}*/

//--------------------------------------------------
vector<string> kuFileReadStrings( string fileName )    //считать текстовый файл в строки
{
    vector<string> lines;
    kuAssert( kuFileExists( fileName ), "cmFileReadStrings no file " + fileName );
    ifstream f(fileName.c_str(),ios::in| ios::binary);
    string line;
    while (getline(f,line)) {
        if (line=="") continue;
        else {
            //убираем в конце '\r' для правильного считывания windows-файлов в linux
            //и в windows также сейчас такие есть
            while ( kuStringLastSymbol( line ) == '\r' ) {
                line = line.substr( 0, line.length() - 1 );
            }
            lines.push_back( line );
        }
     }
    return lines;
}

//--------------------------------------------------
string kuFileReadString( string fileName )    //считать текстовый файл в строку, с контролем \r для Linux
{

    kuAssert( kuFileExists( fileName ), "cmFileReadString - file '" + fileName + "' not found" );

    //TODO на linux последний символ "\r" отрезает всё строку, поэтому такое считывание сделал
    string value = "";
    FILE *f = fopen( fileName.c_str(), "rb" );
    if ( f ) {
        while (!feof(f)) {
            char c;
            fread( &c, 1, 1, f );
            if (feof(f)) break;
            if ( c == '\r' || c == '\n' ) break;
            value += c;
        }
        fclose( f );
    }
    return value;
}

//--------------------------------------------------
void kuFileWriteStrings( const vector<string> &list, string fileName )    //записать строки в текстовый файл
{
    ofstream f(fileName.c_str(),ios::out);
    kuAssert( !f.fail(), "cmFileWriteStrings - error creating file " + fileName );
    for ( size_t i=0; i<list.size(); i++ ) {
        f << list[i] << endl;
    }
    kuAssert( !f.fail(), "cmFileWriteStrings - error writing file " + fileName );
    f.close();
}

//--------------------------------------------------
void kuFileWriteString( const string &line, const string &fileName )    //записать строку в текстовый файл
{
    vector<string> file;
    file.push_back( line );
    kuFileWriteStrings( file, fileName );
}

//--------------------------------------------------
int kuFileReadInt( string fileName ) {
    //vector<string> file = kuFileReadStrings( fileName );
    //kuAssert( file.size() > 0, "cmFileReadInt, bad file " + fileName );
    string file = kuFileReadString( fileName );
    kuAssert( !file.empty(), "cmFileReadInt, bad file " + fileName );
    return kuToInt( file );
}

//--------------------------------------------------
void kuFileWriteInt( int value, string fileName ) {
    vector<string> file;
    file.push_back( kuToString(value) );
    kuFileWriteStrings( file, fileName );

}

//--------------------------------------------------
string kuFetchExePath() //путь к исполняемому файлу
{
#ifdef QT
    QString qs;
    QFileInfo fi(QCoreApplication::applicationFilePath());
    qs = fi.absolutePath();
    string s = qs.toUtf8().constData();
    return s;
#else
    kuAssert( false, "cmGetExePath not implemented" );
	return "";
#endif

}

//--------------------------------------------------
string kuPathToWindows( string path ) {
    kuStringReplace( path, "/", "\\" );
    return path;
}

//--------------------------------------------------
string kuPathToLinux( string path ) {
    kuStringReplace( path, "\\", "/" );
    return path;
}

//--------------------------------------------------
string kuPathToQt( string path ) { //сконвертировать путь для Qt
    return kuPathToLinux( path );
}

//--------------------------------------------------
string kuPathToOS( string path ) //сконвертировать путь по ОС (\ в Windows, / в Linux)
{
#ifdef OS_WINDOWS
    return kuPathToWindows( path );
#endif
#ifdef OS_LINUX
    return kuPathToLinux( path );
#endif
    return path;
}

//--------------------------------------------------
/*bool kuMakeDir( string path )  //создать папку
{
    int n = path.length();
    if ( n > 0 ) {
       while ( path[n-1] == '/' || path[n-1] == '\\' ) {
           path = path.substr( 0, n-1 );
       }
       //Версия Windows и без проверки
       //string command = "mkdir " + path;
       //kuLog << "Make dir by command " << command << endl;
       //system( command.c_str() );
       //return true;

       //Версия Qt
       path = kuPathToQt( path );
       QDir dir( path.c_str() );
       if (!dir.exists()) {
           dir.mkpath(".");
       }
       return dir.exists();

       //kuAssert( QDir( path.c_str() ).exists(), "cmMakeDir - can not create directory " + path );
    }
    return true;    //TODO следует поставить false, но нужно всё протестировать!
}

//--------------------------------------------------
bool kuDirRename( string from, string to ) { //переименование папки

    from = kuPathToQt( from );
    to = kuPathToQt( to );
    //QDir dir;//( from.c_str() );
    return QDir().rename( from.c_str(), to.c_str() );
}

//--------------------------------------------------
bool kuDirDelete( string path ) {   //удаление папки
    path = kuPathToQt( path );
    QDir dir( path.c_str() );
    return dir.removeRecursively();
}

//--------------------------------------------------
bool kuDirExists( string path )  //существует ли папка
{
    path = kuPathToQt( path );
    QDir dir( path.c_str() );
    return dir.exists();
}

//--------------------------------------------------
bool kuCd( string path ) {       //перейти в каталог
    path = kuPathToQt( path );
    cout << "cmCd " << path << endl;
    return QDir::setCurrent( path.c_str() );
}

//--------------------------------------------------
bool kuFileCopy( string from, string to ) {  //копирование файла
    from = kuPathToQt( from );
    to = kuPathToQt( to );
    //cout << "------------ from " << from << " to " << to << endl;

    if ( !kuFileExists(from) ) return false;

    bool res = true;
    //нужно удалить файл, так как QFile не может сам перезатереть
    if ( kuFileExists(to) ) {
        res = kuFileDelete(to);
    }
    if ( res ) res = QFile().copy( from.c_str(), to.c_str() );
    //kuLog << "result: " << res << endl;
    return res;
}

//--------------------------------------------------
bool kuFileDelete( string path ) {  //удаление файла
    path = kuPathToQt( path );
    bool res = true;
    if ( kuFileExists( path ) ) {
        res = QFile().remove( path.c_str() );
    }
    if ( res ) res = !kuFileExists( path );
    return res;
}

//--------------------------------------------------
//Cвободное место на диске, из которого запущена программа

#ifdef OS_LINUX
    #include <sys/statvfs.h>
    #include <QDir>
#endif

u64 kuDiskFreeSpace() {
#ifdef OS_WINDOWS
    __int64 FreeBytesAvailable;
    __int64 TotalNumberOfBytes;
    __int64 TotalNumberOfFreeBytes;

    BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx(NULL, // по умолчанию: откуда запущен проект
    (PULARGE_INTEGER)&FreeBytesAvailable, // доступно для использования(байт)
    (PULARGE_INTEGER)&TotalNumberOfBytes, // максимальный объём(в байтах)
    (PULARGE_INTEGER)&TotalNumberOfFreeBytes); // свободно на диске(в байтах)

    kuAssert(GetDiskFreeSpaceFlag != 0, "Function GetDiskFreeSpaceEx is called incorrectly.");

    return FreeBytesAvailable;
#endif

#ifdef OS_LINUX
//    kuExit( "cmDiskFreeSpace is not implemented in Linux!" );
//    return 0;
    struct statvfs stat;
    const char *path = QDir::currentPath().toUtf8().data();
    kuAssert(statvfs(path, &stat) == 0, "Function statvfs is called incorrectly.");
    return stat.f_bsize * stat.f_bavail;
#endif
}*/

//--------------------------------------------------
//------------------------------------BinC--------------
//--------------------------------------------------
void kuStringToBuffer( string s, u8 *buffer, int bufferSize )
{
    s = s.substr( 0, bufferSize );
    memset( buffer, 0, bufferSize );
    for ( unsigned int i=0; i<s.length(); i++) {
        buffer[i] = s[i];
    }
}

//--------------------------------------------------
string kuBufferToString( u8 *buffer, int bufferSize )
{
    string s;
    for (int i=0; i<bufferSize; i++) {
        if ( buffer[i] == 0 ) { break; }
        s += buffer[i];
    }
    return s;
}

//--------------------------------------------------
time_t ku_utils_time0 = 0;

void kuSetup() {
    time( &ku_utils_time0 );
}

//--------------------------------------------------
int kuElapsedSecondsi() {
    kuAssert( ku_utils_time0 > 0, "cmElapsedSecondsi not inited, please call kuSetup" );
    time_t t;
    time( &t );
    return difftime( t, ku_utils_time0 );
}

//--------------------------------------------------
/*void kuSleepMs(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
*/
//--------------------------------------------------
string kuTimeString() {  //текущее время, в виде строки
    return //QDateTime::currentDateTime().toString().toUtf8().constData();
		"TIME";
}
//--------------------------------------------------
bool kuIsInt(string s){
    kuAssert(s.length()!=0, "empty line in kuIsInt");
    vector<bool> znach;
    bool ret_znach = true;
    size_t pos = s.find(".",0);
    if (pos==string::npos) {
        for (int i=0; i<s.length(); i++){
            if((s[i]=='-'||s[i]=='+')&&i==0) continue;
            else {
                if (s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9')
                    znach.push_back(true);
                else {
                    if ((i!=0 || s.length()==1)&& s[i]=='0') znach.push_back(true);
                    else{
                    znach.push_back(false);
                    //cout<<"false1"<<endl;
                    }
                }
                if (i==0){
                    if(s[i]=='0') {
                        if (s.length()==1) znach.push_back(true);
                        else{
                        znach.push_back(false);
                        //cout<<"false2"<<endl;
                        }
                    }
                }
                else if((s[i-1]=='-'||s[i-1]=='+')&& s[i]=='0') {
                    znach.push_back(false);
                    //cout<<"false3"<<endl;
                }
                     else znach.push_back(true);
            }
        }
        for (int j=0; j<znach.size(); j++){
            if(znach[j]==false) ret_znach = false;
        }
    }
    else {
        ret_znach=false;
        //cout<<"false4"<<endl;
    }
    return ret_znach;
}
//-------------------------------------------------
bool kuIsFloat(string s){
    bool ret_znach=true;
    vector<bool> znach;
    kuAssert (s.length()!=0, "empty line in kuIsFloat");
    size_t pos = s.find(".",0);
    if (pos!=string::npos) {
        size_t pos1 = s.find(".",pos+1);
        if (pos1!=string::npos) znach.push_back(false);
        else{
            if (pos==0) znach.push_back(false);
            else{
                for (int i=0; i<pos; i++){
                    int size = s.substr(0,pos).size();
                    if (s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9'||(s[i]=='0'&& (size==1 ||size==2))
                                                                                                                 ||(s[i]=='+'&&i==0)||(s[i]=='-' && i==0))
                        znach.push_back(true);
                    else znach.push_back(false);
                }
                for (int j=pos+1; j<s.length(); j++){
                    if (s[j]=='1'||s[j]=='2'||s[j]=='3'||s[j]=='4'||s[j]=='5'||s[j]=='6'||s[j]=='7'||s[j]=='8'||s[j]=='9'||s[j]=='0')
                        znach.push_back(true);
                    else znach.push_back(false);
                }
            }

        }

    }
    else znach.push_back(false);
    for (int k=0; k<znach.size(); k++){
        if (znach[k]==false) ret_znach=false;
    }
    return ret_znach;
}
