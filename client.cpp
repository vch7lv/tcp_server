#include "LinuxTCPSocket.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage ./client ip port\n";
        return 0;
    }

    LinuxTCPSocket socket;

    try
    {
        socket.connect(get_ipv4_address(argv[1], atoi(argv[2])));
        char buf[256] = "hello, server\n";
        socket.write(buf, sizeof(buf));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}