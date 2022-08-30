/**
 * @file server.cpp
 * @author leroncheung
 * @brief 多进程版本，可服务多个客户端进程的连接请求并与之通信
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server.h>

Server::~Server() {
    CloseServerSocket();
}

bool Server::InitServer(const std::string& port) {
    act_.sa_handler = read_child_proc;  // 注册信号处理函数
    sigemptyset(&act_.sa_mask);
    act_.sa_flags = 0;
    // 子进程结束后由操作系统发送SIGCHLD信号给父进程，然后执行注册的新号处理函数，及时销毁子进程所占用的资源，避免变成僵尸进程。
    if (sigaction(SIGCHLD, &act_, 0) == -1) {
        std::cerr << "sigaction failed!" << std::endl;
        return false;
    }

    if (server_sockfd_ >= 0) {
        CloseServerSocket();
    }

    server_sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sockfd_ == -1) {
        perror("socket failed");
        return false;
    }

    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr_.sin_port = htons(atoi(port.c_str()));

    if (bind(server_sockfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) {
        perror("bind failed");
        CloseServerSocket();
        return false;
    }

    if (listen(server_sockfd_, 5) < 0) {
        perror("listen failed");
        CloseServerSocket();
        return false;
    }

    std::cout << "The server initialization is complete! Start listening..." << std::endl;
    return true;
}

bool Server::Accept() {
    if (server_sockfd_ < 0) {
        std::cerr << "The server socket has not created!" << std::endl;
        return false;
    }

    client_addr_len_ = sizeof(client_addr_);
    client_sockfd_ = accept(server_sockfd_, (struct sockaddr *)&client_addr_, &client_addr_len_);
    if (client_sockfd_ < 0) {
        perror("accept failed");
        return false;
    }
    std::cout << "The client [" << inet_ntoa(client_addr_.sin_addr) << "] has connected server!" << std::endl;
    return true;
}

int Server::StartFork() {
    while (true) {
        std::cout << "Waiting for connecting..." << std::endl;

        if (!Accept()) {
            continue;
        }

        pid_ = fork();
        if (pid_ == 0) { // 子进程的操作区域
            std::cout << "Child's pid = " << pid_ <<" and accept client socket file descriptor = " << client_sockfd_ << std::endl;
            CloseServerSocket();  // 关闭从父进程中拷贝的服务端socket，该socket用来accept其他客户端的连接请求，子进程中不再需要
            int buf_len = 0;
            char buff[BUF_SIZE];    // 父进程不需要buff，因此将buff字符数组定义在子进程中，而没有定义为Server类成员变量
            while (tcp_base_recv(client_sockfd_, buff, &buf_len)) {
                buff[buf_len] = 0;
                std::cout << "Receievd msg: " << buff << std::endl;
                sprintf(buff, "ok");    // 服务器回复给客户端：ok
                if (!tcp_base_send(client_sockfd_, buff)) {
                    break;
                }
            }
            std::cout << "Child's pid [" << pid_ << "] closed accept client socket [" << client_sockfd_ << "]." << std::endl;
            CloseClientSocket();
            return 10;
        } else { // 父进程的操作区域
            CloseClientSocket();    // accept_client_socket是服务器端创建的socket，用来与客户端进行通信，父进程中不需要，直接关闭
        }
    }
    CloseServerSocket();
    return 0;
}

void Server::CloseServerSocket() {
    if (server_sockfd_ >= 0) {
        close(server_sockfd_);
    }
    server_sockfd_ = -1;
}

void Server::CloseClientSocket() {
    if (client_sockfd_ >= 0) {
        close(client_sockfd_);
    }
    client_sockfd_ = -1;
}