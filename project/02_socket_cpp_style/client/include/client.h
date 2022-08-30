/**
 * @file client.h
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

const int BUFF_SIZE = 1024;

/**
 * @brief 客户端类：封装了部分函数，供外部直接调用
 * 
 */
class Client {
public:
    Client() = default;
    virtual ~Client() = default;
    Client(const Client &client) = default;
    Client &operator=(const Client &client) = default;
    Client(Client &&client) = default;
    Client &operator=(Client &&client) = default;

    /**
     * @brief 客户端初始化：创建客户端的套接字
     * 
     * @return true ：创建成功
     * @return false ：创建失败
     */
    bool InitClient();

    /**
     * @brief 使用提供的端口号尝试去连接服务器（主机名或IP地址指定）的监听套接字：调用connect系统调用
     * 
     * @param hostname_or_ip_addr ：主机名或IP地址的字符串
     * @param port ：端口号字符串
     * @return true ：连接成功
     * @return false ：连接失败
     */
    bool Connect(const std::string &hostname_or_ip_addr, const std::string &port);

    /**
     * @brief 接收服务器端的TCP字节流消息
     * 
     * @return int ：接收到的消息长度
     */
    int Recv();

    /**
     * @brief 发送TCP消息给服务器端
     * 
     * @param str ：待发送的字符串
     */
    void Send(const std::string& str);

    /**
     * @brief 关闭客户端套接字
     * 
     */
    void CloseClientSocket();

private:
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    char buff_[BUFF_SIZE];

    /**
     * @brief Get the Host By Name object：内部调用gethostbyname()系统调用查看主机名对应的IP地址
     * 
     * @param hostname_or_ip_addr ：参数为主机名或者IP地址均可
     * @return in_addr ：返回的是IP地址结构体
     */
    in_addr GetHostByName(const std::string& hostname_or_ip_addr) const;
};

#endif  // _CLIENT_H_