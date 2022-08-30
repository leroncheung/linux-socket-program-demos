/**
 * @file base_util.cpp
 * @author leroncheung
 * @brief 基础工具库函数源代码：主要解决TCP消息的发送和接收时的分包和粘包问题。
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <base_util.h>

bool base_recv(const int socket_fd, char* buff, const size_t len) {
    ssize_t total_size = len;   // ssize_t 为有符号整型， size_t 为无符号整型
    ssize_t total_received_size = 0;

    while (total_size > 0) {
        ssize_t received_size = recv(socket_fd, buff + total_received_size, total_size, 0);
        if (received_size <= 0) {
            perror("recv failed");
            return false;
        }
        total_received_size += received_size;
        total_size -= received_size;
    }
    return true;
}

bool base_send(const int socket_fd, const char* buff, const size_t len) {
    ssize_t total_size = len;
    ssize_t total_sent_size = 0;
    while (total_size > 0) {
        ssize_t sent_size = send(socket_fd, buff + total_sent_size, total_size, 0);
        if (sent_size <= 0) {
            perror("send failed");
            return false;
        }
        total_sent_size += sent_size;
        total_size -= sent_size;
    }
    return true;
}

bool tcp_base_recv(const int socket_fd, char* buff, int* buff_len) {
    if (socket_fd < 0) {
        return false;
    }
    *buff_len = 0;
    if (!base_recv(socket_fd, (char*)buff_len, LEN_SIZE)) {    // 先读取固定4字节的长度数据，保存在buff_len中
        return false;
    }
    *buff_len = ntohl(*buff_len);                   // 网络字节序转换为主机字节序
    if (!base_recv(socket_fd, buff, *buff_len)) {   // 再读取长度为buff_len的真实数据包长，解决粘包问题
        return false;
    }
    return true;
}

bool tcp_base_send(const int socket_fd, const char* buff, const int buf_len) {
    if (socket_fd < 0) {
        return false;
    }
    int len = buf_len;
    if (buf_len == 0) {
        len = strlen(buff);
    }

    char str_buff[len + LEN_SIZE];
    memset(str_buff, 0, sizeof(str_buff));

    int len_net = htonl(len);                  // 将数据长度转换为网络字节序
    memcpy(str_buff, &len_net, LEN_SIZE);      // 将转换为网络字节序的数据长度当作“数据”写入到缓存str_buff中
    memcpy(str_buff + LEN_SIZE, buff, len);    // 将待发送的数据内容写入到str_buff中，紧跟在数据长度后。解决分包问题

    if (!base_send(socket_fd, str_buff, len + LEN_SIZE)) {
        return false;
    }
    return true;
}

void read_child_proc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);  // WNOHANG表示非阻塞运行该函数
    if (WIFEXITED(status)) {                    // 在父进程读取子进程的退出状态后，操作系统会销毁子进程的相关资源
        std::cout << "Status of child's process: " << WEXITSTATUS(status) << ". Remove child's process: " << pid << std::endl;
    }
}