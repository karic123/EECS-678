//Main.cpp
#include <iostream>
#include "Socket.h"

using namespace std;

int main()
{
    int choice;
    int port = 666;
    //char *ipAddress = "127.0.0.1";
    string ipAddress;
    bool done = false;
    char recMessage[STRLEN];
    char sendMessage[STRLEN];
    cout<<"1) Host server"<<endl;
    cout<<"2) Join server"<<endl;
    cout<<"3) Quit"<<endl;
    cin>>choice;
    if ( choice == 3 )
        exit(0);
    else if ( choice == 2 )
    {
        //Client
        cout<<"Enter an IP address, 127.0.0.1 is the loopback address"<<endl;
        cin>>ipAddress;
        ClientSocket sockClient;
        cout<<"ATTEMPTING TO CONNECT..."<<endl;
        sockClient.ConnectToServer( ipAddress.c_str(), port );
        //Connected
        while ( !done )
        {     
            sockClient.GetAndSendMessage();
            cout<<"\t--WAIT--"<<endl;
            sockClient.RecvData( recMessage, STRLEN );
            cout<<"Recv > "<<recMessage<<endl;
            if ( strcmp( recMessage, "end" ) == 0 ||
                        strcmp( sendMessage, "end" ) == 0 )
            {
                done = true;
            }
        }
        sockClient.CloseConnection();
    }
    else if ( choice == 1 )
    {
        //SERVER
        ServerSocket sockServer;
        cout<<"HOSTING..."<<endl;
        sockServer.StartHosting( port );
        //Connected
        while ( !done )
        {
            cout<<"\t--WAIT--"<<endl;
            sockServer.RecvData( recMessage, STRLEN );
            cout<<"Recv > "<<recMessage<<endl;
            sockServer.GetAndSendMessage();
            if ( strcmp( recMessage, "end" ) == 0 ||
                    strcmp( sendMessage, "end" ) == 0 )
            {
                done = true;
            }
        }
    }
}










