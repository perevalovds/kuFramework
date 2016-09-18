#pragma once

//Логи

#include <kuConstants.h>
#include <kuUtils.h>

using namespace std;

//Код выхода из программы по умолчанию
//Когда pt получает код, >= 1001, он считает что это ошибка была сгенерирована нашими модулями,
//и они уже выдали диалоговое сообщениме
#define ku_exit_code (1001)

//размер изображения для логов
#define kuLog_ImageSize (10000000)

//Лог для отладочных сообщений
#define kuLog cout << "log: "

//Лог для вывода пользователю
#define kuLogScreen cout << "screen: "

//Диалог
#define kuLogMessageBox cout << "MESSAGE_DLG: "

//Диалог ошибки
#define kuLogError cout << "ERROR_DLG: "

#define kuLogIf( condition, text ) { if (condition) kuLog text; }

//Лог про операции загрузки/записи изображений
#define kuLogImg( w, h, text ) { kuLogIf( w*h > kuLog_ImageSize, text ) }

//Прогресс-бар
#define kuLogProgress cout << "PROGRESS: "


//---- контроль ошибок ----
void kuLogAppend(const string &message , const string &message_type = "ERROR" );  //добавить строку в log
void kuExit( string message, int exitCode = ku_exit_code, bool print_Error_Dlg = true );
void kuExitSilent( int exitCode );
//void kuAssert( bool value, string message );
#define kuAssert( value, message ) if (!(value)) kuExit( (message) ); else {}

//Запомнить команду, которая была вызвана, чтобы ее занести в файл в случае ошибки
void kuLog_store_command( string kud );

void kuExitDisable(bool v);	
//Выходить ли из программы в случае ошибки.
//Если выключено - то, последняя ошибка хранится в ku_error_string,
//а флаг была ли ошибка - в ku_error_flag

void ku_error_reset();
bool ku_error_flag();
string ku_error_string();
