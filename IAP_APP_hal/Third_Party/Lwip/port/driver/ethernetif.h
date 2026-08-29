#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

/* Maximum Transfer Unit */
#define NETIF_MTU						( 1500 )
#define NETIF_IN_TASK_STACK_SIZE		( 1024 )
#define NETIF_IN_TASK_PRIORITY			( 3 )




/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);
void ethernetif_input( void *argument );
void ethernetif_output( void *argument );
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);



#endif 
