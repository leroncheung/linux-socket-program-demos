/**
 * @file server_epoll.h
 * @author leroncheung
 * @brief epoll函数封装类ServerEpoll头文件
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_EPOLL_H_
#define _SERVER_EPOLL_H_

#include <sys/epoll.h>
#include <unistd.h>

const int EPOLL_SIZE = 100;

class ServerEpoll {
public:
    ServerEpoll();
    virtual ~ServerEpoll();
    ServerEpoll(ServerEpoll &server_epoll) = default;
    ServerEpoll &operator=(const ServerEpoll &&server_epoll) = default;
    ServerEpoll(ServerEpoll &&server_epoll) = default;
    ServerEpoll &operator=(ServerEpoll &&server_epoll) = default;

    /**
     * @brief 将sock_fd添加到监测事件中
     * 
     * @param sock_fd ：socket文件描述符
     * @return int ：返回状态值
     */
    int AddEpollEvent(const int sock_fd);

    /**
     * @brief 将sock_fd从监测事件中删除
     * 
     * @param sock_fd ：socket文件描述符
     * @return int ：返回状态值
     */
    int DeleteEpollEvent(const int sock_fd);

    /**
     * @brief 内部执行epoll_wait，开始监测socket文件描述符
     * 
     * @param events ：监测的事件
     * @return int ：返回发生变化的事件的数量
     */
    int EpollWait(epoll_event* events);

private:
    struct epoll_event event_;
    int epoll_fd_;
};

#endif // _SERVER_EPOLL_H_