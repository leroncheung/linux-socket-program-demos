/**
 * @file main.cpp
 * @author leroncheung
 * @brief main program of server app:单进程版服务器代码
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Examples: ./server 5001" << std::endl;
        return -1;
    }

    Server server;
    if (!server.InitServer(argv[1])) {
        return -1;
    }

    if (!server.Accept()) {
        return -1;
    }

    while (server.Recv() > 0) {
        server.Send();
    }
    server.CloseClientSocket();
    server.CloseServerSocket();

    return 0;
}