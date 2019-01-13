#include <string.h>
#include <unistd.h>
#include "broadcast.h"
#include "debug.h"

int main()
{
        int sockfd = broadcast_recvsocket_create(54545);
        broadcast_socket_setnonblock(sockfd);

        char buffer[2048] = "";
        char sendBuffer[2048] = "";
        char sendIP[512] = "";

        while(1)
        {
                memset(buffer, 0, sizeof(buffer));
                memset(sendBuffer, 0, sizeof(sendBuffer));
                memset(sendIP, 0, sizeof(sendIP));

                sleep(1);
                be_printf("等待广播消息\n");
                int rv = broadcast_socket_recv(sockfd, buffer);
                if(rv < 0)
                {
                        be_printf("timeout\n");
                        continue;
                }
                be_printf("RECV:%s\n", buffer);
                get_broadcast_senderIP(sendIP);

                //TODO
                strcpy(sendBuffer,"hello\n");
                // (shell-command msg #t)


                int ret = create_tcp_send_message(sendIP, 54546, sendBuffer,strlen(sendBuffer));
                if(ret <= 0)
                        be_printf("tcp send error!");
                else
                        be_printf("SEND:%s\n", sendBuffer);
        }

        return 0;
}
