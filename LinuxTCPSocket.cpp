
#include "LinuxTCPSocket.h"
#include <iostream>

LinuxTCPSocket::LinuxTCPSocket () : LinuxTCPSocket(get_any_ipv4_address())
{}

LinuxTCPSocket::LinuxTCPSocket (int sd) :  sd(sd), blocking(fcntl(sd,F_GETFL) & O_NONBLOCK)
{}

LinuxTCPSocket::LinuxTCPSocket (sockaddr_in adr, bool blocking) : blocking(blocking)
{
    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd == -1) 
    {
        std::cout << "123";
        throw LinuxTCPSocketException{"failed to create socket(): " + errno};
    }
    
    set_blocking(blocking);

    if (bind(sd, (sockaddr*)(&adr), sizeof(adr)) == -1)
    {
        std::cout << "3";

        throw LinuxTCPSocketException{"failed to bind(): " + errno};
    }
}

LinuxTCPSocket::~LinuxTCPSocket()
{
    if (sd)
    {
        close(sd);
    }
}



void LinuxTCPSocket::connect(sockaddr_in server_adr) 
{
    if (::connect(sd, (sockaddr*)&server_adr, sizeof(server_adr)) == -1)
    {
        throw LinuxTCPSocketException{"connect() failed: " + errno};
    }
}

void LinuxTCPSocket::listen(int backlog) 
{
    if (::listen(sd, backlog) == -1)
    {
        throw LinuxTCPSocketException{"listen() failed: " + errno};
    }
}

std::unique_ptr<LinuxTCPSocket> LinuxTCPSocket::accept() const 
{
    sockaddr_in adr;
    socklen_t len = sizeof(adr);
    int client_sd = ::accept(sd, (sockaddr*)&adr, &len);

    if (!blocking && client_sd == -1 && errno == EAGAIN)
    {
        return nullptr;
    }

    if (client_sd == -1)
    {
        throw LinuxTCPSocketException{"accept() failed: " + errno};
    }

    return std::make_unique<LinuxTCPSocket>(client_sd);
}

void LinuxTCPSocket::set_blocking(bool f) 
{
    int prev_flags = fcntl(sd, F_GETFL);
    int new_flags = (f ? prev_flags & ~O_NONBLOCK : prev_flags | O_NONBLOCK);
    fcntl(sd, F_SETFL, new_flags);
}

ssize_t LinuxTCPSocket::read (void* buf, size_t sz)
{
    return ::read(sd, buf, sz);
}
ssize_t LinuxTCPSocket::write (void* buf, size_t sz)
{
    return ::write(sd, buf, sz);
}



sockaddr_in get_any_ipv4_address()
{
    sockaddr_in adr;

    adr.sin_family = AF_INET;
    adr.sin_port = 0;
    adr.sin_addr.s_addr = INADDR_ANY;

    return adr;
}

sockaddr_in get_ipv4_address(uint32_t ip, uint16_t port) 
{
    sockaddr_in adr;

    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = htonl(ip);

    return adr;
}

sockaddr_in get_ipv4_address(const std::string& ip, uint16_t port) 
{
    sockaddr_in adr;
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    
    if (inet_aton(ip.c_str(), &adr.sin_addr) == 0)
    {
        throw LinuxTCPSocketException{"wrong string of ip address\n"};
    }

    return adr;
}



LinuxTCPSocketException::LinuxTCPSocketException(const std::string& str) : message(str)
{}

const char* LinuxTCPSocketException::what() const noexcept
{
    return message.c_str();
}