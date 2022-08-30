/**
 * @file client.cpp
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <client.h>

Client::~Client() {
    CloseClientSock();
}

bool Client::InitClient() {
    if (client_sockfd_ != -1) {
        CloseClientSock();
    }

    client_sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_sockfd_ == -1) {
        perror("socket failed");
        return false;
    }
    std::cout << "The client initialization is complete!" << std::endl;
    return true;
}

in_addr Client::GetHostByName(const std::string& hostname_or_ip_addr) const {
    struct in_addr sin_addr;
    memset(&sin_addr, 0, sizeof(sin_addr));
    struct hostent *host = gethostbyname(hostname_or_ip_addr.c_str());
    if (host == nullptr) {
        perror("gethostbyname");
        return sin_addr;
    }
    memcpy(&sin_addr, host->h_addr, host->h_length);
    return sin_addr;
}

bool Client::Connect(const std::string& hostname_or_ip_addr, const std::string& port) {
    if (client_sockfd_ < 0) {
        std::cout << "The client socket has not created!" << std::endl;
        return false;
    }
    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr = GetHostByName(hostname_or_ip_addr);
    server_addr_.sin_port = htons(atoi(port.c_str()));

    if (connect(client_sockfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) {
        perror("connect failed");
        CloseClientSock();
        return false;
    }
    std::cout << "The client has connected server ["<< hostname_or_ip_addr << "]!" << std::endl;
    return true;
}

int Client::Recv() {
    if (client_sockfd_ < 0) {
        std::cout << "The client socket has not created!" << std::endl;
        return -1;
    }
    memset(buff_, 0, sizeof(buff_));
    // ssize_t recv_len = recv(client_sockfd_, &buff_, BUFF_SIZE, 0);
    if (!tcp_base_recv(client_sockfd_, buff_, &buff_len_)) {   // 为了解决TCP粘包问题
        CloseClientSock();
        return -1;
    }
    std::cout << "Received msg: " << buff_ << std::endl;
    return buff_len_;
}

void Client::Send(const std::string& msg) {
    if (client_sockfd_ < 0) {
        std::cout << "The client socket has not created!" << std::endl;
        return;
    }
    // send(client_sockfd_, &buff_, strlen(buff_), 0);
    if (tcp_base_send(client_sockfd_, msg.c_str(), msg.size())) {   // 为了解决TCP分包问题
        std::cout << "Send msg succeed!" << std::endl;
    }
}

void Client::CloseClientSock() {
    if (client_sockfd_ >= 0) {
        close(client_sockfd_);
    }
    client_sockfd_ = -1;
}
