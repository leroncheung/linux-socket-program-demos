# Conclusion
> server.cpp client.cpp

## server.cpp


## client.cpp


## struct
1. sockaddr_in
    > 需将主机字节转换成的大端字节序后保存：htons, htonl, inet_addr
    - sin_family
    - sin_addr
        - s_addr
    - sin_port
    - sin_len
2. sockaddr
3. 

## functions
1. int socket(int domain, int type, int protocol);
    > 返回文件描述符
    - domain协议族: AF_INET、AF_INET6
    - type：SOCK_STREAM、SOCK_PACKET
    - protocol：IPPROTO_TCP、IPPROTO_UDP、IPPROTO_STCP
2. int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
3. int listen(int sockfd, int backlog);
4. int connect(int sockfd, struct sockaddr * serv_addr, int addrlen);
5. int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
6. ssize_t send(int sockfd, const void *buf, size_t len, int flags);
7. ssize_t recv(int sockfd, void *buf, size_t len, int flags);
8. struct hostent *gethostbyname(const char *name);
9. uint16_t htons(uint16_t hostshort);
    > host to network short: 将主机的无符号short类型数值转换成网络字节序，即大端模式
10. uint32_t htonl(uint32_t hostlong);
    > host to network long: 将主机的无符号long类型数值转换成网络字节序，即大端模式
11. int atoi(const char *str);
12. int close(int fd);