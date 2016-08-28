#include "kuLog.h"

string kuLog_stored_command_;
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
