/**
 * @file server_select.h
 * @author leroncheung
 * @brief 基于select函数的I/O复用头文件
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_SELECT_H_
#define _SERVER_SELECT_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/select.h>
#include <sys/time.h>

class ServerSelect {
public:
    // 建议提供无参数的构造函数，否则编译器不会主动提供（因为已经提供了有参构造函数）。此时类成员变量的初始值由类内初始值确定。
    ServerSelect() = default;
    explicit ServerSelect(const int server_socket_fd);
    virtual ~ServerSelect() = default;
    ServerSelect(const ServerSelect &server_select) = default;
    ServerSelect &operator=(const ServerSelect &server_select) = default;
    ServerSelect(ServerSelect &&server_select) = default;
    ServerSelect &operator=(ServerSelect &&server_select) = default;

    /**
     * @brief 注册文件描述符socket_fd的信息（设置对应的文件描述符比特位置为1），加入监听
     *
     * @param socket_fd ： 需要被监听的socket文件描述符
     */
    void AddSocketFd(const int socket_fd);

    /**
     * @brief 清除注册的文件描述符fd信息（设置文件描述符对应比特位置上的1为0），取消监听
     * 
     * @param socket_fd ：取消监听的socket文件描述符
     */
    void DeleteSocketFd(const int socket_fd);

    /**
     * @brief 执行select函数，开始监听
     * 
     * @param vec_fd_bits 
     * @return int 
     */
    int SelectWait(std::vector<int>& vec_fd_bits);

private:
    int max_fd_ = 0;    // C++11：类内初始值
    fd_set read_fd_set_;
    // fd_set write_fd_set_;   // 后续扩充select的监听范围使用：是否可传输无阻塞数据
    // fd_set except_fd_set_;  // 后续扩充select的监听范围使用：是否发生异常
    fd_set temp_fd_set_;
    struct timeval timeout_;
};

#endif // _SERVER_SELECT_H_