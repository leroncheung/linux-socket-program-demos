/**
 * @file server.h
 * @author leroncheung
 * @brief epoll版服务器头文件
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <base_util.h>
#include <server_epoll.h>

const int BUFF_SIZE = 1024;

class Server {
public:
    Server() = default;
    virtual ~Server();
    Server(const Server &server) = delete;
    Server &operator=(const Server &server) = delete;
    Server(Server &&server) = default;
    Server &operator=(Server &&server) = default;

    /**
     * @brief 初始化服务器：创建socket，命名socket，监听socket
     * 
     * @param port ：端口字符串
     * @return true ：初始化成功
     * @return false ：初始化失败
     */
    bool InitServer(const std::string& port);

    /**
     * @brief 开始执行epoll_wait函数
     * 
     */
    void StartEpoll();


private:
    int server_sockfd_ = -1;
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    socklen_t client_addr_len_ = 0;
    int buf_len_ = 0;

    epoll_event epoll_events[EPOLL_SIZE];
    ServerEpoll server_epoll_;

    /**
     * @brief 内部执行系统调用accept
     * 
     * @return true ：连接成功，创建client_sockfd_
     * @return false ：连接失败
     */
    bool Accept();

    /**
     * @brief 关闭server_sockfd_
     * 
     */
    void CloseServerSocket();
};

#endif // _SERVER_H_