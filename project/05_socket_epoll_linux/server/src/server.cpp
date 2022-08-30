/**
 * @file server.cpp
 * @author leroncheung
 * @brief epoll版服务器源文件
 * @version 0.1
 * @date 2022-08-17
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

    if (listen(server_sockfd_, 5) == -1) {
        perror("listen failed");
        CloseServerSocket();
        return false;
    }
    
    if (server_epoll_.AddEpollEvent(server_sockfd_) < 0) {
        std::cerr << "The server socket registered failed!" << std::endl;
        CloseServerSocket();
        return false;
    }

    std::cout << "The server Initialization is complete! Wait for connecting..." << std::endl;
    return true;
}

bool Server::Accept() {
    if (server_sockfd_ < 0) {
        std::cerr << "The server socket has not created!" << std::endl;
        return false;
    }
    client_addr_len_ = sizeof(client_addr_);
    client_sockfd_ = accept(server_sockfd_, (struct sockaddr *)&client_addr_, &client_addr_len_);
    if (client_sockfd_ == -1)
    {
        perror("accept failed");
        CloseServerSocket();
        return false;
    }
    std::cout << "The client [" << inet_ntoa(client_addr_.sin_addr) << "] has connected server!" << std::endl;
    return true;
}

void Server::StartEpoll() {
    while (true) {
        int event_cnt = server_epoll_.EpollWait(epoll_events);
        if (event_cnt < 0) {
            std::cerr << "epoll wait occured error!" << std::endl;
            break;
        }
        for (int i = 0; i < event_cnt; i++) {
            int socket_fd = epoll_events[i].data.fd;
            if (socket_fd == server_sockfd_) {
                if (!Accept()) {
                    break;
                }
                server_epoll_.AddEpollEvent(client_sockfd_);
                continue;
            }

            char buff[BUFF_SIZE];
            int buf_len;

            if (!tcp_base_recv(socket_fd, buff, &buf_len)) {
                std::cout << "The client has closed socket!" << std::endl;
                server_epoll_.DeleteEpollEvent(socket_fd);
                close(socket_fd);
            } else {
                buff[buf_len] = 0;
                std::cout << "Received msg: " << buff << std::endl;
                sprintf(buff, "ok");
                if (!tcp_base_send(socket_fd, buff)) {
                    std::cout << "The client has closed socket!" << std::endl;
                    server_epoll_.DeleteEpollEvent(socket_fd);
                    close(socket_fd);
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