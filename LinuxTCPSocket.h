#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdint>
#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>


class LinuxTCPSocket
{
    int sd = -1;
    bool blocking;

    public:
        LinuxTCPSocket ();
        LinuxTCPSocket (int sd);
        LinuxTCPSocket (sockaddr_in adr, bool blocking = true);
        ~LinuxTCPSocket();

        void connect(sockaddr_in server_adr);
        void listen(int backlog);
        std::unique_ptr<LinuxTCPSocket> accept() const;

        ssize_t read (void* buf, size_t sz);
        ssize_t write (void* buf, size_t sz); 

        void set_blocking(bool flag);
};


sockaddr_in get_any_ipv4_address();
sockaddr_in get_ipv4_address(uint32_t ip, uint16_t port);
sockaddr_in get_ipv4_address(const std::string& ip, uint16_t port);



class LinuxTCPSocketException : public std::exception
{
    std::string message;

    public:
        LinuxTCPSocketException(const std::string& str);
        virtual const char* what() const noexcept;
};

