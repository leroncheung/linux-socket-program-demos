/**
 * @file server.h
 * @author leroncheung
 * @brief header of server app
 * @version 0.1
 * @date 2022-08-10
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUFF_SIZE = 1024;

class Server {
public:
    Server() = default;
    virtual ~Server() = default;
    Server(const Server &server) = delete;      // 禁止拷贝构造函数
    Server &operator=(const Server &server) = delete; // 禁止拷贝赋值运算符函数
    Server(Server &&server) = default;  // 使用编译器提供的默认移动构造函数
    Server &operator=(Server &&server) = default;   // 使用编译器提供的默认移动赋值运算符函数

    /**
     * @brief 初始化Server：创建socket，绑定socket，监听socket
     * 
     * @param port为端口号字符串
     * @return true：初始化成功
     * @return false：初始化失败
     */
    bool InitServer(const std::string& port);

    /**
     * @brief 内部调用accept系统调用：完成客户端的连接请求，并建立与此客户端通信的客户端socket文件描述符（即client_sockfd_成员）
     * 
     * @return true ：连接建立成功
     * @return false ：连接建立失败
     */
    bool Accept();

    /**
     * @brief 内部调用recv系统调用，完成tcp消息的接收
     * 
     * @return int ：返回接收成功的消息长度
     */
    int Recv();

    /**
     * @brief 内部调用send系统调用，完成tcp消息（暂时指定为字符串"ok"）的发送
     * 
     */
    void Send();

    /**
     * @brief 关闭服务器的套接字server_sockfd_
     * 
     */
    void CloseServerSocket();

    /**
     * @brief 关闭服务器端建立的与客户端通信的套接字client_sockfd_
     * 
     */
    void CloseClientSocket();

private:
    int server_sockfd_ = -1;
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    struct sockaddr_in client_addr_;
    socklen_t client_addr_len_ = 0;
    char buff_[BUFF_SIZE];
};

#endif // _SERVER_H_