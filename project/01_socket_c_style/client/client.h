/**
 * @file client.h
 * @author leroncheung
 * @brief header of client
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <string.h> // memset, mmecpy
#include <stdlib.h> // atoi
#include <sys/socket.h>
#include <netdb.h> // gethostbyname, struct sockaddr_in, htons, htonl
#include <arpa/inet.h> // sockaddr_in, htons, htonl, inet_ntoa
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFF_SIZE 1024
#define NUMBER_OF_MSGS 5

#endif // _CLIENT_H_