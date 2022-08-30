/**
 * @file server.h
 * @author leroncheung
 * @brief 多线程版服务器端头文件
 * @version 0.1
 * @date 2022-08-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <base_util.h>

const int BUFF_SIZE = 1024;
const int MAX_CLIENT = 10;

struct socket_descriptor {
    int id_ = -1;
    int sock_fd_ = -1;
    std::string ip_addr_;
};

class Server {
public:
    Server() = default;
    virtual ~Server();
    Server(const Server &server) = delete;
    Server &operator=(const Server &server) = delete;
    Server(Server &&server) = delete;
    Server &operator=(Server &&server) = delete;

    /**
     * @brief 初始化服务器：创建socket，命名socket，监听socket
     * 
     * @param port ：端口字符串
     * @return true ：初始化成功
     * @return false ：初始化失败
     */
    bool InitServer(const std::string& port);

    /**
     * @brief 从主线程中创建新线程同时将客户端的信息传递过去，使之通信
     * 
     */
    void StartCreateThread();

private:
    int server_sockfd_ = -1;
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    socklen_t client_len_;
    int id_ = 0;
    std::mutex mutex_;  // TODO

    /**
     * @brief 内部调用accept系统调用，建立与客户端的连接
     * 
     * @return true ：建立成功
     * @return false ：建立失败
     */
    bool Accept();

    /**
     * @brief 新线程入口函数，负责与连接的客户端进行通信
     * 
     * @param sd ：连接的客户端的socket信息
     */
    void EchoThread(socket_descriptor& sd);

    /**
     * @brief 关闭server_sockfd_
     * 
     */
    void CloseServerSocket();
};

#endif // _SERVER_H_