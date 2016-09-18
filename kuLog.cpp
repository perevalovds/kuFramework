#include "kuLog.h"

string kuLog_stored_command_;

bool ku_exit_disabled_ = false;
bool ku_error_flag_ = false;
string ku_error_string_;

//----------------------------------------
void kuExitDisable(bool v) {
	ku_exit_disabled_ = v;
}


//----------------------------------------
void ku_error_reset() {
	ku_error_flag_ = false;
	ku_error_string_ = "";
}

//----------------------------------------
bool ku_error_flag() {
	return ku_error_flag_;
}

//----------------------------------------
string ku_error_string() {
	return ku_error_string_;
}

//----------------------------------------
void kuLog_store_command( string kud ) {
    kuLog_stored_command_ = kud;
}

//----------------------------------------
void kuLogAppend( const string &message, const string &message_type ) {  //добавить строку в log
    //запись в файл
    string time = kuTimeString();

    string text = time + "\t" + message_type + ": " + message;
    //выводим в файл команду, которая вызывалась
    if ( !kuLog_stored_command_.empty() ) {
        text += + "\n\t from command: " + kuLog_stored_command_;
    }
    kuFileAppendText( ".log", text );
}

//----------------------------------------
void kuExit( string message, int exitCode, bool print_Error_Dlg ) {
	//режим без выхода из программы
	if (ku_exit_disabled_) {
		cout << "ERROR: " << message << endl;
		ku_error_flag_ = true;
		ku_error_string_ = message;
		return;
	}
    //kuLog << "============= Error " << message.c_str() << endl;
    if ( exitCode != 0 ) {
        if ( print_Error_Dlg ) {
            kuLogError << message.c_str() << endl;
        }
        kuLogAppend( message );
    }

    kuLog << "--- kum exiting with code " << exitCode << endl;
    exit( exitCode );
}

//--------------------------------------------------
void kuExitSilent( int exitCode ) {
    //kuFileAppendText( ".log", "Silent exit" );
    exit( exitCode );
}

//--------------------------------------------------
//void kuAssert( bool value, string message )
//{
//    if ( !value ) {
//        //kuExit( "Assertion: " + message );
//        kuExit( message );
//    }
//}

//--------------------------------------------------
