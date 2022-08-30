/**
 * @file base_util.h
 * @author leroncheung
 * @brief 基础工具库函数的头文件
 * @version 0.1
 * @date 2022-08-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _BASE_UTIL_H_
#define _BASE_UTIL_H_

#include <iostream>
#include <string>
#include <sys/wait.h>
#include <arpa/inet.h>

const int LEN_SIZE = 4;     // 用4字节表示包长，通过该长度进行发送和接收，避免出现分包和粘包

/**
 * @brief 对于发送端发送过来的总长度为len的tcp字节流，需要1次或多次调用recv系统调用才能将读缓存全部读完，并保存在buff指向的内存中
 * 
 * @param socket_fd ：socket文件描述符
 * @param buff ：接收数据的内存地址
 * @param len ：需要读取的总长度，不能超过buff的大小，否则会溢出。使用const标记是因为传入参数可能为常量。
 * @return true ：1次或多次recv后数据完整读取出来并保存在buff中
 * @return false ：recv调用失败
 */
bool base_recv(const int socket_fd, char* buff, const size_t len);

/**
 * @brief 将buff中的总长度为len的字节信息发送到对端，需要1次或多次调用send系统调用才能全部发送完毕
 * 
 * @param socket_fd ：socket文件描述符
 * @param buff ：待发送的消息保存位置
 * @param len ：待发送的消息总长度
 * @return true ：发送成功
 * @return false ：发送失败
 */
bool base_send(const int socket_fd, const char* buff, const size_t len);

/**
 * @brief 先读取4字节（LEN_SIZE）固定长度的包长数据，再通过该包长数据去读取实际的数据内容，有效解决了粘包问题
 * 
 * @param socket_fd ：socket文件描述符
 * @param buff ：保存接收到的数据
 * @param buff_len ：实际从缓存中读取的数据长度，指针类型可以修改指向内存的值
 * @return true ：读取成功
 * @return false ：读取失败
 */
bool tcp_base_recv(const int socket_fd, char* buff, int* buff_len);

/**
 * @brief 先将数据总长度填入待发送的缓存中，紧跟着再填入数据部分，有效解决了分包问题
 * 
 * @param socket_fd ：socket文件描述符
 * @param buff ：待发送的数据
 * @param buf_len ：待发送的数据长度，默认值为0，则实际发送选择buff的长度作为数据总长度
 * @return true ：发送成功
 * @return false ：发送失败
 */
bool tcp_base_send(const int socket_fd, const char* buff, const int buf_len = 0);

/**
 * @brief 父进程读取子进程退出状态的函数
 * 
 * @param sig：用于sigaction函数用于注册信号使用，表示相关的信号，必须保留
 */
void read_child_proc(int sig);

#endif // _BASE_UTIL_H_