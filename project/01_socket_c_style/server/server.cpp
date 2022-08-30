/**
 * @file server.cpp
 * @author leroncheung
 * @brief source code of server
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Example: ./server 5000\n");
        return -1;
    }

    // 1.创建Socket
    // file descriptor文件描述符，一般从3开始。0，1，2已被系统占用，0:标准输出，1:标准输入，2:标准错误
    // int socket(int domain, int type, int protocol);
    // domain协议族: AF_INET、AF_INET6
    // type：SOCK_STREAM、SOCK_PACKET
    // protocol：IPPROTO_TCP、IPPROTO_UDP、IPPROTO_STCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1) {
        perror("sokcet failed");
        return -1;
    }

    // 2.给Socket绑定IP地址和端口
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 若不用htonl，则对应的client端也不用。若一方使用，则两端都要使用。转换成网络字节序函数。
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind failed");
        close(socket_fd);
        return -1;
    }

    // 3.把Socket设置为监听模式：listen，此时若client发起连接请求，则可以正常建立连接，并将连接请求放入连接队列中
    if (listen(socket_fd, 5) == -1) {
        perror("listen failed");
        close(socket_fd);
        return -1;
    }
    printf("Server init succeed! Waiting for connecting...\n");

    // 4.等待并接受client的连接请求
    struct sockaddr_in client_addr;   // client_addr用来保存accept中连接server的client端的IP地址等信息。若用不到，也可以不保存（accept中参数传0即可）。
    int socket_len = sizeof(struct sockaddr_in);
    // 从已经准备好的连接队列中获取一个请求，若队列为空，accept函数将阻塞等待
    int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, (socklen_t *)&socket_len);
    printf("Client [%s] has connected to server!\n", inet_ntoa(client_addr.sin_addr));

    // 5.与连接的client进行通信：接收和发送消息
    char buffer[BUFF_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t recv_len = recv(client_fd, buffer, BUFF_SIZE, 0);
        if (recv_len == 0) {
            printf("The client [%s] has closed socket!\n", inet_ntoa(client_addr.sin_addr));
        } else if (recv_len < 0) {
            perror("receive failed");
            break;
        }
        printf("Server received msg: %s\n", buffer);

        strcpy(buffer, "ack, next");
        if (send(client_fd, buffer, strlen(buffer), 0) <= 0) {
            perror("send failed");
            break;
        }
        printf("Server sent msg: %s\n", buffer);
    }

    // 6.关闭Socket，释放资源
    close(socket_fd);   // 监听socket
    close(client_fd);   // accept函数创建的用于与client收发消息的socket

    return 0;
}