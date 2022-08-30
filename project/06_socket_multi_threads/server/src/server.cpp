/**
 * @file server.cpp
 * @author leroncheung
 * @brief 多线程版服务器源文件
 * @version 0.1
 * @date 2022-08-21
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
        perror("bind failed");
        CloseServerSocket();
        return false;
    }

    std::cout << "The server initialization is complete! Start listening..." << std::endl;
    return true;
}

void Server::EchoThread(socket_descriptor& sd) {
    std::cout << "New thread [" << std::this_thread::get_id() << "]" << std::endl;
    std::cout << "Socket file descriptor: " << sd.sock_fd_ << "  IP addr: "
              << sd.ip_addr_ << "  id: " << sd.id_ << std::endl;
    
    int buf_len = 0;
    char buff[BUFF_SIZE];
    while (tcp_base_recv(sd.sock_fd_, buff, &buf_len)) {
        buff[buf_len] = 0;
        std::cout << "Received msg: " << buff << std::endl;
        sprintf(buff, "ok");
        if (!tcp_base_send(sd.sock_fd_, buff)) {
            break;
        }
    }
    std::cout << "The client [" << sd.ip_addr_ << "] has closed socket!" << std::endl;
    close(sd.sock_fd_);
}

bool Server::Accept() {
    if (server_sockfd_ < 0) {
        std::cerr << "The server socket has not created!" << std::endl;
        return false;
    }
    client_len_ = sizeof(client_addr_);
    client_sockfd_ = accept(server_sockfd_, (struct sockaddr *)&client_addr_, &client_len_);
    if (client_sockfd_ < 0) {
        perror("accept failed");
        return false;
    }
    std::cout << "The client [" << inet_ntoa(client_addr_.sin_addr) << "] has connected server!" << std::endl;
    return true;
}

void Server::StartCreateThread() {
    while (true) {
        std::cout << "Waiting for connecting..." << std::endl;
        if (!Accept()) {
            continue;
        }

        socket_descriptor sd;
        sd.sock_fd_ = client_sockfd_;
        sd.ip_addr_ = inet_ntoa(client_addr_.sin_addr);
        sd.id_ = id_;

        std::thread thread_one(&Server::EchoThread, this, std::ref(sd));  // 创建新线程与客户端进行通信
        thread_one.detach();

        id_++;
    }
}
void Server::CloseServerSocket() {
    if (server_sockfd_ >= 0) {
        close(server_sockfd_);
    }
    server_sockfd_ = -1;
}