#include "main.h"
#include "./app/tcp_client.h"
#include "lwip/api.h"
#include "lwip/sockets.h"


/**
  * @brief 
  * @param 
  * @retval 
  */
static void tcp_client_thread(void)
{
    int sockdes = -1;
    struct sockaddr_in xclient_addr;

    uint8_t recvbuf[256];

    while(1)
    {
        sockdes = socket(AF_INET, SOCK_STREAM, 0);
        if(sockdes < 0)
        {
            printf("Socket created failed");
            vTaskDelay(10);
            continue;
        }
        xclient_addr.sin_family = AF_INET;
        xclient_addr.sin_port = htons(DEST_PORT);
        xclient_addr.sin_addr.s_addr = inet_addr(DEST_IP_ASCII);
        memset(&(xclient_addr.sin_zero), 0, sizeof(xclient_addr.sin_zero));

        if(connect(sockdes, (struct sockaddr*)&xclient_addr, sizeof(struct sockaddr)) != 0)
        {
            printf("Socket connected failed");
            closesocket(sockdes);
            vTaskDelay(10);
            continue;
        }
        printf("Socket connected success");

        while(1)
        {
            if(read(sockdes, recvbuf, sizeof(recvbuf)) > 0){
                Usart_SendString(recvbuf);
            }else{
                break;
            }

            vTaskDelay(1000);
        }
        closesocket(sockdes);
    }
    
}

void client_init(void)
{
    sys_thread_new("tcp_client_thread", (lwip_thread_fn)tcp_client_thread, NULL, 512, 4);
}

