/**
 * @file client.h
 * @author leroncheung
 * @brief 
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <base_util.h>

const int BUFF_SIZE = 1024;

class Client {
public:
    Client() = default;
    virtual ~Client();
    Client(const Client &client) = default;
    Client &operator=(const Client &client) = default;
    Client(Client &&client) = default;
    Client &operator=(Client &&client) = default;

    /**
     * @brief 初始化客户端：创建socket套接字
     * 
     * @return true ：创建套接字成功
     * @return false ：创建套接字失败
     */
    bool InitClient();

    /**
     * @brief 向指定的服务器端发起连接请求
     * 
     * @param hostname_or_ip_addr ：主机名或IP地址的字符串
     * @param port ：端口号字符串
     * @return true ：连接成功
     * @return false ：连接失败
     */
    bool Connect(const std::string& hostname_or_ip_addr, const std::string& port);

    /**
     * @brief 内部调用base_util库的tcp_base_recv函数，解决了粘包问题
     * 
     * @return int ：返回正确接收的数据总长度
     */
    int Recv();

    /**
     * @brief 内部调用base_util库的tcp_base_send函数，解决了分包问题
     * 
     * @param msg ：待发送的消息
     */
    void Send(const std::string& msg);

private:
    int client_sockfd_ = -1;
    struct sockaddr_in server_addr_;
    char buff_[BUFF_SIZE];
    int buff_len_ = 0;

    /**
     * @brief 封装了gethostbyname系统调用
     * 
     * @param hostname_or_ip_addr ：主机名或IP地址的字符串
     * @return in_addr ：in_addr格式的IP地址，已转换为网络字节序
     */
    in_addr GetHostByName(const std::string &hostname_or_ip_addr) const;

    /**
     * @brief 关闭client_sockfd_;
     *
     */
    void CloseClientSock();
};

#endif // _CLIENT_H_