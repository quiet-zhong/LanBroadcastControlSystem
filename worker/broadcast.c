#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "debug.h"

#define BUFF_SIZE 1024

//发送端广播地址
struct sockaddr_in broadcast_addr;

//用于接收端保存广播者的信息
struct sockaddr_in g_src_addr;

int broadcast_sendsocket_create(char *groupIP, int port)
{
        //sender的socket
        int socket_fd;

        //用于配置socket参数
        int opt_so_broadcast= 1;

        //初始化socket
        socket_fd = socket(PF_INET,SOCK_DGRAM,0);
        if(socket_fd < 0)
        {
                be_printf("%s::socket error\n",__func__);
                return socket_fd;
        }

        memset(&broadcast_addr,0,sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_addr.s_addr = inet_addr(groupIP);
        broadcast_addr.sin_port = htons(port);

        //设置socket可选项，因为默认生成的会阻止广播，所以要将可选项中的SO_BROADCAST标志置为1
        if(-1 == setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,(void *)&opt_so_broadcast,sizeof(opt_so_broadcast)))
        {
                be_printf("%s::setsockopt error\n",__func__);
                close(socket_fd);
                return -1;
        }

        return socket_fd;
}

int broadcast_recvsocket_create(int port)
{
        int socket_fd;
        struct sockaddr_in receiver_addr;

        //初始化socket
        socket_fd = socket(PF_INET,SOCK_DGRAM,0);
        if(socket_fd < 0)
        {
                be_printf("%s::socket error\n",__func__);
                return socket_fd;
        }
        memset(&receiver_addr,0,sizeof(receiver_addr));
        receiver_addr.sin_family = AF_INET;
        receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        receiver_addr.sin_port = htons(port);

        //绑定地址
        if(bind(socket_fd,(struct sockaddr *)&receiver_addr,sizeof(receiver_addr)) == -1)
        {
                be_printf("bind() error\n");
                close(socket_fd);
                return -1;
        }

        return socket_fd;
}

int broadcast_socket_send(int socket_fd, char *buff)
{
        be_printf("send:%s",buff);
        //将数据发送到多播组
        return sendto(socket_fd,buff,strlen(buff),0,(struct sockaddr *) &broadcast_addr,sizeof(broadcast_addr));
}


int broadcast_socket_setnonblock(int sockfd)
{
        int flag = fcntl(sockfd, F_GETFL, 0);
        if (flag < 0)
        {
                be_printf("%s:: fcntl error 11\n",__func__);
                return flag;
        }

        flag = fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
        if (flag < 0)
        {
                be_printf("%s:: fcntl error 22\n",__func__);
        }
        return flag;
}

int broadcast_socket_recv(int socket_fd,char *rbuf)
{
        int str_len;
        char buff[BUFF_SIZE];

        int addr_len = sizeof(struct sockaddr_in);

        memset(buff,0,sizeof(buff));
        memset(&g_src_addr,0,addr_len);
        str_len = recvfrom(socket_fd,buff,BUFF_SIZE-1,0,(struct sockaddr *)&g_src_addr ,&addr_len);
        if(str_len < 0)
                return -1;

        memcpy(rbuf,buff,str_len);
        return str_len;
}

void broadcast_socket_recv_getinfo(char *ip,int *port)
{
        *port = broadcast_addr.sin_port;
        strcpy(ip, broadcast_addr.sin_addr.s_addr);
}

int broadcast_socket_destroy(int socket_fd)
{
        return close(socket_fd);
}

void  get_broadcast_senderIP(char *ipAddress)
{
        strcpy(ipAddress, (char *)inet_ntoa(g_src_addr.sin_addr));
        return;
}

int create_tcp_send_message(char *ip,int port,char *message,int len)
{
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd <= 0)
                return -1;

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(ip);
        //inet_pton(AF_INET, ip, &servaddr.sin_addr); //ipV6

        int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if(ret != 0)
        {
                perror("connectBloking");
                close(sockfd);
                return -1;
        }
        ret = send(sockfd, message, len, 0);
        close(sockfd);
        return ret;
}
