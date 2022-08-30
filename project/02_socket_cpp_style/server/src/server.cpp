/**
 * @file server.cpp
 * @author leroncheung
 * @brief source code of server app
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server.h>

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
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);   // 允许服务器接收来自任意IP地址端发送的连接请求
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
    if (client_sockfd_ == -1) {
        perror("accept failed");
        CloseServerSocket();    // 单进程版服务器作简易化处理：accept失败后，直接关闭服务器套接字。不再尝试其他客户端的连接请求
        return false;
    }
    std::cout << "The client [" << inet_ntoa(client_addr_.sin_addr) << "] has connected server!" << std::endl;
    return true;
}

int Server::Recv() {
    if (client_sockfd_ < 0) {
        std::cerr << "There has no clients connected!" << std::endl;
        return 0;
    }
    memset(&buff_, 0, sizeof(buff_));
    ssize_t recv_len = recv(client_sockfd_, buff_, BUFF_SIZE, 0);
    if (recv_len > 0) {
        std::cout << "Received msg: " << buff_ << std::endl;
    } else if (recv_len == 0) {
        std::cout << "Client [" << inet_ntoa(client_addr_.sin_addr) << "] has closed socket!" << std::endl;
        CloseClientSocket();
    } else {
        perror("recv failed");
        CloseClientSocket();
    }
    return recv_len;
}

void Server::Send() {
    if (client_sockfd_ < 0) {
        std::cerr << "There has no clients connected!" << std::endl;
        return;
    }
    sprintf(buff_, "ok");
    if (send(client_sockfd_, buff_, strlen(buff_), 0) < 0)
    {
        perror("send failed");
        return;
    }
    std::cout << "Send msg: " << buff_ << std::endl;
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
