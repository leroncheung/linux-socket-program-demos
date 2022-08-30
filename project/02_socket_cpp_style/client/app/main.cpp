/**
 * @file main.cpp
 * @author leroncheung
 * @brief 使用Client类的一个客户端demo
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <client.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Examples: ./client 127.0.0.1 5001" << std::endl;
        return -1;
    }

    Client client;
    if (!client.InitClient()) {
        return -1;
    }

    if (!client.Connect(argv[1], argv[2])) {
        return -1;
    }

    std::cout << "Input msg(Exit:q): " << std::endl;
    std::string msg;
    while (std::cin >> msg && msg != "q") {
        client.Send(msg);
        if (client.Recv() <= 0) {
            break;
        }
    }
    client.CloseClientSocket();

    return 0;
}