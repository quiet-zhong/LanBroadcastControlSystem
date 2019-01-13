#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef DEBUG

int g_log_fd = 0;

void open_log_file()
{
        g_log_fd =  open("./debug.log", O_CREAT | O_RDWR | O_TRUNC , 0666);
        if(g_log_fd < 3)
        {
                printf("open logfile error\n");
                while(1) usleep(1000*1000);
        }
}

void be_printf (char *str, ...)
{
        int	len;
        va_list	vl;
        char	buf[4096];

        va_start (vl, str);
        len = vsnprintf (buf, sizeof (buf), str, vl);
        va_end (vl);
        buf[sizeof (buf) - 1] = '\0';
        write (1, buf, len);
        if(g_log_fd != 0)
                write (g_log_fd,buf,len);
        return;
}

void print_buffer(char* msg,unsigned char* pBuff, unsigned long ulLen)
{
        unsigned long ulTmp = 0;
        be_printf("%s:\n", msg);
        be_printf("    ");
        while(ulTmp < ulLen)
        {
                be_printf("%02x ",pBuff[ulTmp]);
                ulTmp++;
                if(!(ulTmp%16))
                {
                        be_printf("\n");
                        be_printf("    ");
                }
        }
        be_printf("\n");
}
#else
void open_log_file(){}
void be_printf(char *fmt,...){ }
void print_buffer(char* msg,unsigned char* pBuff, unsigned long ulLen){}
#endif
