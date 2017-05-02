#include "socket.hpp"

Socket::Socket()
    : BUFFER_SIZE( 256 )
{
    m_socketAddress.sun_family = AF_UNIX;
}

Socket::~Socket()
{
    Close();
}

bool Socket::Setup( const string& address )
{
    SetAddress( address );
    return true;
}

void Socket::SetAddress( const string& address )
{
    m_address = address;
    strcpy( m_socketAddress.sun_path, m_address.c_str() );
}

string Socket::Read()
{
    char buffer[ BUFFER_SIZE ];
    string strBuffer = "";

    int bytesRead = read( m_sessionFileDescriptor, buffer, BUFFER_SIZE );
    strBuffer += buffer;

//    while ( bytesRead > 0 )
//    {
//        strBuffer += buffer;
//        // Keep reading
//        bytesRead = read( m_sessionFileDescriptor, buffer, BUFFER_SIZE );
//    }

    return strBuffer;
}

void Socket::Write( const string& text )
{
//    Report( "Writing: " + text );
    write( m_sessionFileDescriptor, text.c_str(), sizeof( text.c_str() ) );
}

void Socket::Report( const string& text )
{
    cout << "[" << m_name << "]: " << text << endl;
}

void Socket::Close()
{
    close( m_sessionFileDescriptor );
}

/* CLIENT *****************************************/

ClientSocket::ClientSocket()
{
    m_name = "CLIENT";
}

bool ClientSocket::Setup( const string& address )
{
    Socket::Setup( address );

    bool okState;

    okState = Socket();
    if ( !okState ) { cerr << "Error on Socket()!" << endl; return okState; }

    okState = Connect();
    if ( !okState ) { cerr << "Error on Socket()!" << endl; return okState; }

    return okState;
}

bool ClientSocket::Socket()
{
    Report( "Socketing..." );
    m_sessionFileDescriptor = socket( PF_UNIX, SOCK_STREAM, 0 );
    return ( m_sessionFileDescriptor >= 0 );
}

bool ClientSocket::Connect()
{
    Report( "Connecting..." );
    int result = connect( m_sessionFileDescriptor, (struct sockaddr*)(&m_socketAddress), sizeof( m_socketAddress ) );
    return ( result >= 0 );
}

/* SERVER *****************************************/

ServerSocket::ServerSocket()
{
    m_name = "SERVER";
}

bool ServerSocket::Setup( const string& address )
{
    Socket::Setup( address );

    bool okState;

    okState = Handshake();
    if ( !okState ) { cerr << "Error on Handshake()!" << endl; return okState; }

    okState = Bind();
    if ( !okState ) { cerr << "Error on Bind()!" << endl; return okState; }

    okState = Listen();
    if ( !okState ) { cerr << "Error on Listen()!" << endl; return okState; }

    okState = Accept();
    if ( !okState ) { cerr << "Error on Accept()!" << endl; return okState; }

    return okState;
}

bool ServerSocket::Handshake()
{
    Report( "Handshaking..." );
    m_handshakeFileDescriptor = socket( PF_UNIX, SOCK_STREAM, 0 );
    unlink( m_address.c_str() );
    return ( m_handshakeFileDescriptor >= 0 );
}

bool ServerSocket::Bind()
{
    Report( "Binding..." );
    int result = bind( m_handshakeFileDescriptor, (struct sockaddr*)(&m_socketAddress), sizeof( m_socketAddress ) );
    return ( result >= 0 );
}

bool ServerSocket::Listen()
{
    Report( "Listening..." );
    int result = listen( m_handshakeFileDescriptor, 1337 );
    return ( result >= 0 );
}

bool ServerSocket::Accept()
{
    Report( "Accepting..." );
    m_sessionFileDescriptor = accept( m_handshakeFileDescriptor, NULL, NULL );
    return ( m_sessionFileDescriptor >= 0 );
}

void ServerSocket::Close()
{
    Report( "Closing..." );
    Socket::Close();
    close( m_handshakeFileDescriptor );
}
