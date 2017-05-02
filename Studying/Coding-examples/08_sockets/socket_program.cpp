#include <iostream>
#include <string>
using namespace std;

#include "socket.hpp"

enum SessionType { SERVER, CLIENT };

SessionType GetSessionType();
int MainMenu();

Socket* SetupServer();
Socket* SetupClient();

int main( int args, char* argv[] )
{
    Socket* socket = nullptr;
    SessionType sessionType;

    if ( args == 2 )
    {
        string typeArg( argv[1] );

        if ( typeArg == "-s" )
        {
            sessionType = SERVER;
        }
        else if ( typeArg == "-c" )
        {
            sessionType = CLIENT;
        }
    }
    else
    {
        sessionType = GetSessionType();
    }

    // Set up
    if ( sessionType == SERVER )
    {
        socket = SetupServer();
    }
    else
    {
        socket = SetupClient();
    }

    bool done = false;
    string buffer;

    // Test code
//    while ( !done )
//    {
//        if ( sessionType == SERVER )
//        {
//            buffer = socket->Read();
//        }
//        else
//        {
//            socket->Write( "Hi" );
//        }
//    }

    while ( !done )
    {
        cout << endl << "SEND: ";
        getline( cin, buffer );

        if ( buffer == "QUIT" )
        {
            pid_t byeChild = fork();
            if ( byeChild == 0 )
            {
                socket->Write( "@BYE" );
                done = true;
                break;
            }
            done = true;
            break;
        }

        socket->Write( buffer );
        socket->Report( "Sent: " + buffer );

        buffer = socket->Read();
        socket->Report( "Received: " + buffer );

        if ( buffer == "@BYE" )
        {
            socket->Report( "Partner disconnected." );
            done = true;
        }
    }

    socket->Report( "Goodbye." );

    if ( socket != nullptr )
    {
        delete socket;
    }

    return 0;
}

SessionType GetSessionType()
{
    cout << "1. Server" << endl;
    cout << "2. Client" << endl;
    cout << endl;
    cout << ">> ";

    int sessionType;
    cin >> sessionType;
    cin.ignore();

    return ( sessionType == 1 ) ? SERVER : CLIENT;
}

int MainMenu()
{
    cout << endl << endl;
    cout << "1. Read data" << endl;
    cout << "2. Write data" << endl;
    cout << "3. Exit" << endl;

    cout << ">> ";
    int choice;
    cin >> choice;
    cin.ignore();

    return choice;
}

Socket* SetupServer()
{
    Socket* serverSocket = new ServerSocket();

    string address = "cheeseburger";
    bool success = serverSocket->Setup( address );

    if ( !success )
    {
        cout << "Error creating server!" << endl;
        return nullptr;
    }

    return serverSocket;
}

Socket* SetupClient()
{
    Socket* clientSocket = new ClientSocket();

    string address = "cheeseburger";
    bool success = clientSocket->Setup( address );

    if ( !success )
    {
        cout << "Error creating client!" << endl;
        return nullptr;
    }

    return clientSocket;
}
