/**
 * @file server.h
 * @author leroncheung
 * @brief header of server
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <string.h> // memset, memcpy
#include <stdlib.h> // atoi
#include <sys/socket.h> // socket
#include <arpa/inet.h> // sockaddr_in, htons, htonl, inet_ntoa
#include <netdb.h> // sockaddr_in, htons, htonl
#include <unistd.h> // close

#define BUFF_SIZE 1024

#endif // _SERVER_H_