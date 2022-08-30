/**
 * @file server.h
 * @author leroncheung
 * @brief server类头文件
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#include <base_util.h>

const int BUF_SIZE = 1024;

class Server {
public:
    Server() = default;
    virtual ~Server();
    Server(const Server &server) = delete;
    Server &operator=(const Server &server) = delete;
    Server(Server &&server) = default;
    Server &operator=(Server &&server) = default;

    /**
     * @brief 初始化服务器：注册信号处理函数（用来清理僵尸进程），创建socket，命名socket，监听socket
     * 
     * @param port ：端口字符串
     * @return true ：初始化成功
     * @return false ：初始化失败
     */
    bool InitServer(const std::string& port);

    /**
     * @brief 创建子进程：有新的客户端连接进来后，创建子进程与之保持通信。父进程继续监听其他客户端的连接请求
     * 
     * @return int ：子进程的返回值，非0即可
     */
    int StartFork();

private:
    int server_sockfd_ = -1;
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    socklen_t client_addr_len_ = 0;
    pid_t pid_ = 0;
    struct sigaction act_;

    /**
     * @brief 接收来自客户端的连接请求，内部调用accept系统调用，将连接成功生成的socket文件描述符保存在client_sockfd_中
     * 
     * @return true ：连接成功
     * @return false ：连接失败
     */
    bool Accept();

    /**
     * @brief 关闭server_sockfd_
     * 
     */
    void CloseServerSocket();

    /**
     * @brief 关闭client_sockfd_
     * 
     */
    void CloseClientSocket();
};

#endif // _SERVER_H_