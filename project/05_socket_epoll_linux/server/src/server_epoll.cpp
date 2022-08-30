/**
 * @file server_epoll.cpp
 * @author leroncheung
 * @brief epoll函数封装类ServerEpoll源文件
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <server_epoll.h>

ServerEpoll::ServerEpoll() {
    epoll_fd_ = epoll_create(EPOLL_SIZE);
}

ServerEpoll::~ServerEpoll() {
    close(epoll_fd_);
}

int ServerEpoll::AddEpollEvent(const int sock_fd) {
    event_.events = EPOLLIN;
    event_.data.fd = sock_fd;
    return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock_fd, &event_);
}

int ServerEpoll::DeleteEpollEvent(const int sock_fd) {
    return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock_fd, NULL);
}

int ServerEpoll::EpollWait(epoll_event* events) {
    return epoll_wait(epoll_fd_, events, EPOLL_SIZE, -1);
}