/**
 * @file main.cpp
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Example: ./server 5001" << std::endl;
        return -1;
    }

    Server server;
    if (!server.InitServer(argv[1])) {
        return -1;
    }

    int fork_res = server.StartFork();
    std::cout << "Fork res = " << fork_res << std::endl;
    if (fork_res != 0) {
        return fork_res;
    }

    return 0;
}