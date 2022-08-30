/**
 * @file server.cpp
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server.h>

Server::~Server() {
    CloseServerSocket();
}

bool Server::InitServer(const std::string& port) {
    if (server_sockfd_ >= 0) {
        CloseServerSocket();
    }

    server_sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sockfd_ < 0) {
        perror("socket created failed");
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

    if (listen(server_sockfd_, 5) == -1) {
        perror("listen failed");
        CloseServerSocket();
        return false;
    }

    std::cout << "Server init succeed! Waiting for connecting..." << std::endl;
    return true;
}

bool Server::Accept() {
    if (server_sockfd_ < 0) {
        std::cerr << "server socket has not created!" << std::endl;
        return false;
    }
    client_addr_len_ = sizeof(client_addr_);
    client_sockfd_ = accept(server_sockfd_, (struct sockaddr *)&client_addr_, &client_addr_len_);
    if (client_sockfd_ < 0) {
        perror("accept error");
        return false;
    }
    std::cout << "The client [" << inet_ntoa(client_addr_.sin_addr) << "] has connected server!" << std::endl;
    return true;
}

void Server::StartSelect() {
    ServerSelect server_select(server_sockfd_);  // 初始化server_select对象，并将server_sockfd_加入监听

    while (true) {
        std::cout << "Start listening all socket file descriptors..." << std::endl;
        std::vector<int> read_fds;
        int select_res = server_select.SelectWait(read_fds);
        if (select_res == -1) {
            break;
        } else if (select_res == 0) {
            continue;
        }

        for (int fd : read_fds) {
            std::cout << "socket fd: " << fd << std::endl;
            if (fd == server_sockfd_) {
                if (Accept()) {
                    server_select.AddSocketFd(client_sockfd_);
                }
                continue;
            }

            char buff[BUFF_SIZE];
            int buf_len;
            if (!tcp_base_recv(fd, buff, &buf_len)) {   // 读取数据失败
                server_select.DeleteSocketFd(fd);
                close(fd);                              // 直接调用close系统调用关闭对应的client_socket_fd
            } else {                                    // 读取数据成功后立即回复消息
                buff[buf_len] = 0;
                std::cout << "[" << fd << "] received msg: " << buff << std::endl;
                sprintf(buff, "ok");                    // 回复消息：ok
                if (!tcp_base_send(fd, buff)) {
                    server_select.DeleteSocketFd(fd);
                    close(fd);
                }
            }
        }
    }
}
void Server::CloseServerSocket() {
    if (server_sockfd_ >= 0) {
        close(server_sockfd_);
    }
    server_sockfd_ = -1;
}
