#ifndef _BROADCAST_H_
#define _BROADCAST_H_

int broadcast_sendsocket_create(char *groupIP, int port);

int broadcast_recvsocket_create(int port);

int broadcast_socket_send(int socket_fd, char *buff);


int broadcast_socket_setnonblock(int sockfd);

int broadcast_socket_recv(int socket_fd,char *rbuf);

void broadcast_socket_recv_getinfo(char *ip,int *port);

int broadcast_socket_destroy(int socket_fd);

int create_tcp_send_message(char *ip,int port,char *message,int len);

#endif
