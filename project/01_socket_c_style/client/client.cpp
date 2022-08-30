/**
 * @file client.cpp
 * @author leroncheung
 * @brief source code of client
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "client.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Example: ./client 127.0.0.1 5000\n");
        return -1;
    }

    // 1.创建Socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1) {
        perror("socket failed");
        return -1;
    }

    // 2.配置对端服务器的IP地址和端口，并向该服务器发起连接请求
    struct hostent *h = gethostbyname(argv[1]); // gethostbyname: 通过域名获取IP地址的网络字节序，直接提供IP地址也可以
    if (h == NULL) {
        printf("gethostbyname failed!\n");
        close(socket_fd);
        return -1;
    }
    struct sockaddr_in serv_addr;      // C98允许临时变量在使用前再定义该变量
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));   // htons: 将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian),htonl类似，l为long
    memcpy(&serv_addr.sin_addr, h->h_addr, h->h_length);    // h_addr就是h_addr_list[0]

    if (connect(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect failed");
        close(socket_fd);
        return -1;
    }
    printf("Client connect to server [%s] successfully!\n", inet_ntoa(serv_addr.sin_addr));

    // 3.与server进行通信：发送和接收消息（发送和接收的顺序需要和对端进行对齐，必须保证一个发送一个接收）
    char buffer[MAX_BUFF_SIZE];
    for (int i = 0; i < NUMBER_OF_MSGS; i++) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "This is no. %d msg! please confirm!", i);
        if (send(socket_fd, buffer, strlen(buffer), 0) <= 0) {
            perror("send failed");
            break;
        }
        printf("Client sent msg: %s\n", buffer);

        memset(buffer, 0, sizeof(buffer));
        int iret = recv(socket_fd, buffer, sizeof(buffer), 0);
        if (iret < 0) {         // 返回-1表示recv函数出错
            perror("received failed");
            break;
        } else if (iret == 0) { // 返回0时表示对端已关闭socket
            printf("The server [%s] has closed socket!\n", inet_ntoa(serv_addr.sin_addr));
            break;
        }
        printf("Client received msg: %s\n", buffer);
    }

    // 4.关闭Socket，释放资源
    close(socket_fd);

    return 0;
}