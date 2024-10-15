#include "LinuxTCPSocket.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage ./server ip port\n";
        return 0;
    }

    auto adr = get_ipv4_address(argv[1], atoi(argv[2]));
    LinuxTCPSocket socket {adr};

    try
    {
        socket.listen(5);
        auto client_socket = socket.accept();
        char buf[256];
        client_socket->read(buf, sizeof(buf));
        std::cout << buf;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}