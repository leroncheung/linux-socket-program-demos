/**
 * @file server_select.h
 * @author leroncheung
 * @brief 基于select函数的I/O复用源文件
 * @version 0.1
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <server_select.h>

ServerSelect::ServerSelect(const int server_socket_fd) : max_fd_(server_socket_fd) {
    FD_ZERO(&read_fd_set_);
    FD_SET(max_fd_, &read_fd_set_);   // 将初始化的server_socket_fd加入读监视数组（reads_），便于后续处理accept客户端的连接请求
}

void ServerSelect::AddSocketFd(const int socket_fd) {
    FD_SET(socket_fd, &read_fd_set_);
    max_fd_ = std::max(max_fd_, socket_fd);
    std::cout << "Start listening socket [" << socket_fd << "]!" << std::endl;
}

void ServerSelect::DeleteSocketFd(const int socket_fd) {
    FD_CLR(socket_fd, &read_fd_set_);
}

int ServerSelect::SelectWait(std::vector<int>& read_fds) {
    temp_fd_set_ = read_fd_set_;

    timeout_.tv_sec = 5;    // 5秒后select监听超时
    timeout_.tv_usec = 0;

    int iret = select(max_fd_ + 1, &temp_fd_set_, NULL, NULL, &timeout_);   // 两个NULL分别对应：write_fd_set_, except_fd_set_
    if (iret == -1) {
        perror("select failed");
        return - 1;
    } else if (iret == 0) {
        std::cout << "Time out!" << std::endl;
        return 0;
    }

    for (int fd = 0; fd < max_fd_ + 1; fd++) {
        if (FD_ISSET(fd, &temp_fd_set_)) {   // 筛选出待读取数据的文件描述符
            read_fds.emplace_back(fd);
        }
    }
    return 1;
}