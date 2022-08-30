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

#include <client.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Example: ./client 127.0.0.1 5001" << std::endl;
        return -1;
    }

    Client client;
    if (!client.InitClient()) {
        return -1;
    }
    if (!client.Connect(argv[1], argv[2])) {
        return -1;
    }

    std::cout << "Input msg to send(exit:q/Q):" << std::endl;
    std::string msg;
    while (std::cin >> msg && msg != "q") {
        client.Send(msg);
        if (client.Recv() <= 0) {
            break;
        }
    }

    return 0;
}