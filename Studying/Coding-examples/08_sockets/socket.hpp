#ifndef _SOCKET_HPP
#define _SOCKET_HPP

#include <string>
#include <iostream>
using namespace std;

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/*
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
*/

class Socket
{
    public:
    Socket();
    virtual ~Socket();

    string Read();
    void Write( const string& text );

    virtual bool Setup( const string& address );
    virtual void SetAddress( const string& address );
    virtual void Report( const string& text );
    virtual void Close();

    protected:
    sockaddr_un     m_socketAddress;
    int             m_sessionFileDescriptor;
    string          m_address;
    string          m_name;
    const int       BUFFER_SIZE;
};

class ClientSocket : public Socket
{
    public:
    ClientSocket();

    virtual bool Setup( const string& address );

    bool Socket();
    bool Connect();
};

class ServerSocket : public Socket
{
    public:
    ServerSocket();

    virtual bool Setup( const string& address );

    bool Handshake();
    bool Bind();
    bool Listen();
    bool Accept();

    virtual void Close();

    private:
    int             m_handshakeFileDescriptor;
};

#endif
