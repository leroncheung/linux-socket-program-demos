/**
 * @file client.cpp
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <client.h>

bool Client::InitClient() {
    if (client_sockfd_ >= 0) {
        CloseClientSocket();
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
    struct in_addr sin_addr_;
    memset(&sin_addr_, 0, sizeof(sin_addr_));
    struct hostent *host = gethostbyname(hostname_or_ip_addr.c_str());
    if (host == nullptr) {
        perror("gethostbyname");
        return sin_addr_;
    }
    memcpy(&sin_addr_, host->h_addr, host->h_length);
    return sin_addr_;
}

bool Client::Connect(const std::string &hostname_or_ip_addr, const std::string &port) {
    if (client_sockfd_ < 0) {
        std::cerr << "The client socket has not created!" << std::endl;
        return false;
    }
    memset(&server_addr_, 0, sizeof(server_addr_));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr = GetHostByName(hostname_or_ip_addr);
    server_addr_.sin_port = htons(atoi(port.c_str()));
    
    if (connect(client_sockfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) {
        perror("connect failed");
        CloseClientSocket();
        return false;
    }
    std::cout << "The client has connected server ["<< hostname_or_ip_addr << "]!" << std::endl;
    return true;
}

int Client::Recv() {
    if (client_sockfd_ < 0) {
        std::cerr << "The client socket has not created!" << std::endl;
        return -1;
    }
    memset(buff_, 0, sizeof(buff_));
    ssize_t recv_len = recv(client_sockfd_, &buff_, BUFF_SIZE, 0);
    if (recv_len > 0) {
        std::cout << "Received msg: " << buff_ << std::endl;
    } else if (recv_len == 0) {
        std::cout << "server has closed socket!" << std::endl;
        CloseClientSocket();
    } else {
        perror("recv failed");
        CloseClientSocket();
    }
    return recv_len;
}

void Client::Send(const std::string& msg) {
    if (client_sockfd_ < 0) {
        std::cerr << "The client socket has not created!" << std::endl;
        return;
    }
    if (send(client_sockfd_, msg.c_str(), msg.size(), 0) < 0) {
        perror("send failed");
        return;
    }
    std::cout << "Send msg succeed!" << std::endl;
}

void Client::CloseClientSocket() {
    if (client_sockfd_ > 0) {
        close(client_sockfd_);
    }
    client_sockfd_ = -1;
}